#include "processor.hpp"

Processor::Processor( boost::asio::io_service & ios, const short port ) :
ios_( ios ),
socket_( ios_ ),
port_( port ),
num_t_( 2 ) {
	for ( int i = 0; i < num_t_; i++ ) {
		threads_.add_thread( new boost::thread( boost::bind( &Processor::run_, this )));
	}
}

Processor::~Processor( void ) {
	std::cerr << "Closing Connections..." << std::endl;
	for ( auto guest: guests_ ) {
		delete [] guest.first;
	}
	guests_.clear();

	std::cerr << "Deleting processor..." << std::endl;
	threads_.interrupt_all();
	boost::this_thread::sleep( boost::posix_time::millisec(1000));
}

/* ------------------------------ */
/*              Error             */
/* ------------------------------ */

const char * Processor::Error::name( void ) const noexcept { 
	return "error";
}

std::string Processor::Error::message( int ev ) const { 

	/* List of errors: http://www.boost.org/doc/libs/1_61_0/boost/system/error_code.hpp */

	switch( ev ) {
		case boost::system::errc::success:
		return "OK";

		case boost::system::errc::file_exists:
		return "already in use";

		case boost::system::errc::no_such_file_or_directory:
		return "does not exist";

		default:
		return "processor error code not recorded";
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

bool Processor::Equal::operator() ( const char * lhs, const char * rhs ) const noexcept {
	bool result = ( std::strcmp( lhs, rhs )) ? true : false;
	std::cerr << std::boolalpha;
	std::cerr << lhs << " " << rhs << " " << result << std::endl;
	return result;
}

/* ------------------------------ */
/*   Public Interface (Commands)  */
/* ------------------------------ */

void Processor::async_login( Guest::pointer guest, const_buffer alias, on_async_op comp ) {
	do_async_op fn = boost::bind( &Processor::do_login_, this, guest, alias );
	ios_.post( boost::bind( &Processor::add_, this, fn, comp ));
}

void Processor::async_create_channel( Guest::pointer guest, const_buffer channel, on_async_op comp ) {
	do_async_op fn = boost::bind( &Processor::do_create_channel_, this, guest, channel );
	ios_.post( boost::bind( &Processor::add_, this, fn, comp ));
}

void Processor::async_join_channel( const_buffer channel, on_async_op comp ) {
	do_async_op fn = boost::bind( &Processor::do_join_channel_, this, channel );
	ios_.post( boost::bind( &Processor::add_, this, fn, comp ));
}

void Processor::async_close_channel( const_buffer channel, on_async_op comp ) {
	do_async_op fn = boost::bind( &Processor::do_close_channel_, this, channel );
	ios_.post( boost::bind( &Processor::add_, this, fn, comp ));
}

void Processor::async_leave( Guest::pointer guest, on_async_op comp ) {
	do_async_op fn = boost::bind( &Processor::do_leave_, this, guest );
	ios_.post( boost::bind( &Processor::add_, this, fn, comp ));
}

/* ----------------------------------- */
/* Internal Processor Request Helpers  */
/* ----------------------------------- */


/* ------------------------------------------------------------------------- */
error_code Processor::do_login_( Guest::pointer guest, const_buffer data ) {
/* ------------------------------------------------------------------------- */
	error_code 		ec;
	const char * 	alias;

	alias = boost::asio::buffer_cast<const char *>( data );
	/* Check that the alias is OK first */
	{
		scoped_lock lk( guests_m_ );
		auto result = guests_.find( alias );

		/* If it's not, return */
		if ( result != guests_.end() ) {
			std::cerr << "Processor: found: " << alias << std::endl;
			ec.assign( boost::system::errc::file_exists, proc_errc_ );
			return ec;
		}
	}
	/* Alias is OK */
	char * new_ = new char [ 25 ]();
	std::strcpy( new_, alias );
	guest->set_alias( new_ );
	{
		scoped_lock lk( guests_m_ );
		std::cerr << "inserting " << new_ << std::endl;
		guests_.insert( guest_t( new_, guest ));
	}
 	ec.assign( boost::system::errc::success, proc_errc_ ); 
	return ec;
}

/* ---------------------------------------------------------------------------------- */
error_code Processor::do_create_channel_( Guest::pointer guest, const_buffer data ) {
/* ---------------------------------------------------------------------------------- */
	error_code 		ec;
	const char *	channel;

	channel = boost::asio::buffer_cast<const char *>( data );
	{
		scoped_lock lk( channels_m_ );
		auto result = channels_.find( channel );

		if ( result != channels_.end() ) {
			ec.assign( boost::system::errc::file_exists, proc_errc_ );
			return ec;
		}

		Channel new_channel;

		auto new_session = boost::make_shared<Session>(std::move(socket_), new_channel);

		new_channel.join(new_session);

		auto new_result = channels_.insert( channel_t( channel, new_channel ));   // Create channel
		
		if ( new_result.second ) {
			ios_.post( boost::bind( &Processor::do_connect_, this, guest, channel ));
		}
		/*
		auto new_ = channels_.insert( channel_t( channel, Channel() ));   // Create channel*/
		
	}
	ec.assign( boost::system::errc::success, proc_errc_ );

	std::cout << "CHANNEL: " << channel << " CREATED" << std::endl;
	return ec;
}

/* ---------------------------------------------------------------------------------- */
error_code Processor::do_join_channel_( const_buffer data ) {
/* ---------------------------------------------------------------------------------- */
	error_code 		ec;
	const char *	channel;

	channel = boost::asio::buffer_cast<const char *>( data );
	{
		scoped_lock lk( channels_m_ );
		auto result = channels_.find( channel );

		if ( result == channels_.end() ) {
			ec.assign( boost::system::errc::no_such_file_or_directory, proc_errc_ );
			return ec;
		}
		else
		{
			auto new_session = boost::make_shared<Session>(std::move(socket_), channels_[channel]);
			
			channels_[channel].join(new_session);
		}
	}
	ec.assign( boost::system::errc::success, proc_errc_ );
	return ec;
}

/* ---------------------------------------------------------------------------------- */
error_code Processor::do_close_channel_( const_buffer data ) {
/* ---------------------------------------------------------------------------------- */
	error_code 		ec;
	const char *	channel;

	channel = boost::asio::buffer_cast<const char *>( data );
	{

		/* Where does the channel keep it's name? */


		// scoped_lock lk( channels_m_ );
		// auto result = channels_.find( channel );

		// if ( result != channels_.end() ) {
		// 	ec.assign( boost::system::errc::file_exists, proc_errc_ );
		// 	return ec;
		// } else {
		// 	channels_.erase( );   // Create channel
		// }
	}
	ec.assign( boost::system::errc::success, proc_errc_ );
	return ec;
}

/* ---------------------------------------------------- */
error_code Processor::do_leave_( Guest::pointer guest ) {
/* ---------------------------------------------------- */
	error_code ec;
	{
		scoped_lock lk( guests_m_ );

		std::cerr << "guests:" << std::endl;
		for ( auto guest: guests_ ) {
			std::cerr << guest.first << std::endl;
		}
		std::cerr << "end guests\nguests size: " << guests_.size() << std::endl;
		std::cerr << "front: " << guests_.begin()->first << std::endl;
		// auto result = guests_.find( guest->get_alias() );
		auto it = guests_.begin();
		while ( it != guests_.end() ) {
			std::cerr << it->first << " " << guest->get_alias() << std::endl;
			if ( std::strcmp( it->first, guest->get_alias() ) == 0 ) {
				guests_.erase( it );
				ec.assign( boost::system::errc::success, proc_errc_ );
				
				return ec;
			} else {
				it++;
			}
		}
	}
	ec.assign( boost::system::errc::no_such_file_or_directory, proc_errc_ );
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

void Processor::do_connect_( Guest::pointer guest, std::string channel ) {
	tcp::endpoint endpoint( guest->get_address().address(), port_ );
	socket_.async_connect( endpoint, boost::bind( &Processor::on_connect_, this, _1 ));
}

void Processor::on_connect_( error_code ec ) {
	if ( ! ec ) {
		/* Create Session here */
	}
}


