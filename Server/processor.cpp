#include "processor.hpp"

Processor::Processor( boost::asio::io_service & ios ) :
ios_( ios ),
num_t_( 2 ) {
	for ( int i = 0; i < num_t_; i++ ) {
		threads_.add_thread( new boost::thread( boost::bind( &Processor::run_, this )));
	}
}

Processor::~Processor( void ) {
	std::cerr << "Closing Connections..." << std::endl;
	guests_.clear();

	std::cerr << "Deleting processor..." << std::endl;
	threads_.interrupt_all();
	boost::this_thread::sleep( boost::posix_time::millisec(1000));
}

/* ------------------------------ */
/*              Error             */
/* ------------------------------ */

const char * Processor::Error::name( void ) const noexcept { 
	return "proc.result";
}

std::string Processor::Error::message( int ev ) const { 
	switch( ev ) {
		case boost::system::errc::success:
		return "OK";
		break;

		case boost::system::errc::connection_refused:
		return "connection refused";
		break;

		case boost::system::errc::file_exists:
		return "alias already in use";
		break;

		default:
		return "processor error code not recorded";
		break;
	}
}

/* ------------------------------ */
/*           Operation            */
/* ------------------------------ */

Processor::Operation::Operation( 
	io_service & ios, do_async_op func, on_async_op complete ) :
ios_( ios ),
func_( func ),
complete_( complete )
{ }

Processor::Operation::~Operation( void ) { }

void Processor::Operation::run( void ) {
	ios_.post( boost::bind( complete_, func_() ));
}

/* ------------------------------ */
/*           Comparator           */
/* ------------------------------ */

bool Processor::Equal::operator() ( const char * lhs, const char * rhs ) const noexcept{
	bool result = ( std::strcmp( lhs, rhs ) == 0 );
	std::cerr << std::boolalpha;
	std::cerr << lhs << " " << rhs << " " << result << std::endl;
	return ! result;
}

/* ------------------------------ */
/* Public Interface ( Commands )  */
/* ------------------------------ */

void Processor::async_stage( Guest::pointer guest, on_async_op comp ) {
	do_async_op fn = boost::bind( &Processor::do_stage_, this, guest );
	ios_.post( boost::bind( &Processor::add_, this, fn, comp ));
}

void Processor::async_login( Guest::pointer guest, const_buffer buff, on_async_op comp ) {
	do_async_op fn = boost::bind( &Processor::do_login_, this, guest, buff );
	ios_.post( boost::bind( &Processor::add_, this, fn, comp ));
}

void Processor::async_leave( Guest::pointer guest, on_async_op comp ) {
	do_async_op fn = boost::bind( &processor::do_leave_, this, guest );
	ios_.post( boost::bind( &Processor::add_, this, fn, comp ));
}

/* ----------------------------------- */
/* Internal Processor Request Helpers  */
/* ----------------------------------- */


/* ---------------------------------------------------- */
error_code Processor::do_stage_( Guest::pointer guest ) {
/* ---------------------------------------------------- */
	error_code ec;
	{
		scoped_lock lk( stage_m_ );
		auto result = stage_.insert( guest );
		if ( ! result.second ) {
			ec.assign( boost::system::errc::connection_refused, proc_errc_ );
		}
	}
	ec.assign( boost::system::errc::success, proc_errc_ );
	return ec;
}
/* ------------------------------------------------------------------------- */
error_code Processor::do_login_( Guest::pointer guest, const_buffer data ) {
/* ------------------------------------------------------------------------- */
	error_code 		ec;
	const char * 	alias;

	alias = boost::asio::buffer_cast<const char *>( data );
	/* Check that the alias is OK first */
	{
		scoped_lock lk( stage_m_ );
		auto result = guests_.find( alias );

		/* If it's not, return */
		if ( result != guests_.end() ) {
			ec.assign( boost::system::errc::file_exists, proc_errc_ );
			return ec;
		}
	}
	/* Alias is OK */
	{
		scoped_lock lk( stage_m_ );
		stage_.erase( stage_.find( guest ));
	}
	{
		scoped_lock lk( guests_m_ );
		guests_.insert( guest_t( alias, guest ));
	}
 	ec.assign( boost::system::errc::success, proc_errc_ ); 
	return ec;
}

/* ---------------------------------------------------------------------------------- */
error_code Processor::do_create_channel_( Guest::pointer guest, mutable_buffer data ) {
/* ---------------------------------------------------------------------------------- */
	error_code 		ec;
	const char *	channel;

	channel = boost::asio::buffer_cast<const char *>( data );
	{
		scoped_lock lk( channels_m_ );
		auto result = channels_.find( channel );

		if ( result != channels_.end() ) {
			ec.assign( boost::system::errc::file_exists, proc_errc_ );
		} else {
			Channel c;
			channels_.insert( channel_t( channel, Channel() ));   // Create channel

		}
	}
	ec.assign( boost::system::errc::success, proc_errc_ );
	return ec;
}

/* ---------------------------------------------------- */
error_code Processor::do_leave_( Guest::pointer guest ) {
/* ---------------------------------------------------- */
	error_code ec;
	{
		scoped_lock lk( stage_m_ );
		auto result = stage_.insert( guest );
		if ( ! result.second ) {
			ec.assign( boost::system::errc::connection_refused, proc_errc_ );
		}
	}
	ec.assign( boost::system::errc::success, proc_errc_ );
	return ec;
}

/* ----------------------------------- */
/* Internal Processor Utility Helpers  */
/* ----------------------------------- */

void Processor::add_( do_async_op func, on_async_op comp ) {

	Processor::Operation op( ios_, func, comp );
	boost::unique_lock<boost::mutex> lock( ops_m_ );
	bool work = ! ops_.empty();
	ops_.push( op );
	if ( ! work ) {
		cond_.notify_all();
	}

}

void Processor::run_( void ) {

	try {
		while( true ) {
			boost::unique_lock<boost::mutex> lock( ops_m_ );
			while( ops_.empty() )
				cond_.wait( lock );

			Processor::Operation op = ops_.front();
			ops_.pop();
			
			/* Check that thread should continue with operation -- check interruption flag */
			boost::this_thread::sleep( boost::posix_time::millisec(10));
			op.run();
		}
	} catch ( boost::thread_interrupted& e ) {
		std::cerr << "Processor: thread " << boost::this_thread::get_id() << " terminating." << std::endl;
	}
}




