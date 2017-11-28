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
	std::cerr << "Deleting processor..." << std::endl;
	guests_.clear();
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
	return ( std::strcmp( lhs, rhs )) ? true : false;
}

/* ------------------------------ */
/*   Public Interface (Commands)  */
/* ------------------------------ */
/* -------------------------------------------------------------------------------------- */
void Processor::async_login( Guest::pointer guest, const_buffer alias, on_async_op comp ) {
/* -------------------------------------------------------------------------------------- */
	do_async_op fn = boost::bind( &Processor::do_login_, this, guest, alias );
	ios_.post( boost::bind( &Processor::add_, this, fn, comp ));
}

/* -------------------------------------------------------------------------------------------------- */
void Processor::async_create_channel( Guest::pointer guest, const_buffer channel, on_async_op comp ) {
/* -------------------------------------------------------------------------------------------------- */
	do_async_op fn = boost::bind( &Processor::do_create_channel_, this, guest, channel );
	ios_.post( boost::bind( &Processor::add_, this, fn, comp ));
}

/* ----------------------------------------------------------------------------------------------- */
void Processor::async_join_channel( Guest::pointer guest, const_buffer channel, on_async_op comp ) {
/* ----------------------------------------------------------------------------------------------- */
	do_async_op fn = boost::bind( &Processor::do_join_channel_, this, guest, channel );
	ios_.post( boost::bind( &Processor::add_, this, fn, comp ));
}

/* -------------------------------------------------------------------------- */
void Processor::async_close_channel( const_buffer channel, on_async_op comp ) {
/* -------------------------------------------------------------------------- */
	do_async_op fn = boost::bind( &Processor::do_close_channel_, this, channel );
	ios_.post( boost::bind( &Processor::add_, this, fn, comp ));
}

/* ------------------------------------------------------------------ */
void Processor::async_leave( Guest::pointer guest, on_async_op comp ) {
/* ------------------------------------------------------------------ */
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
	std::cerr << "Processor::do_login_(): users alias: " << alias << std::endl;
	/* Check that the alias is OK first */
	{
		scoped_lock lk( guests_m_ );
		auto result = guests_.find( alias );

		/* If it's not, return */
		if ( result != guests_.end() ) {
			ec.assign( boost::system::errc::file_exists, proc_errc_ );
			return ec;
		}
	}
	/* Alias is OK */
	// char * new_ = new char [ 25 ]();
	guest->set_alias( alias );
	// std::strcpy( new_, alias );

	{
		scoped_lock lk( guests_m_ );
		guests_.insert( guest_t( guest->get_alias(), guest ));
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
		//auto result = channels_.find( channel );

		for (int i = 0; i < channels_.size(); i++)
		{
			if (channels_[i].first == channel)
			{
				ec.assign( boost::system::errc::file_exists, proc_errc_ );
				return ec;
			}
		}

		auto new_channel = boost::make_shared<Channel>( channel );
		auto new_session = boost::make_shared<Session>( guest, std::move( socket_ ), port_, new_channel );
		new_session->start();
		
		channels_.push_back( std::make_pair( new_channel->name(), new_channel ));
		std::cerr << "Processor: do_create_channel_: " << new_channel->name() << std::endl;
	}
	ec.assign( boost::system::errc::success, proc_errc_ );

	std::cout << "CHANNEL: " << channel << " CREATED" << std::endl;
	return ec;
}

/* ---------------------------------------------------------------------------------- */
error_code Processor::do_join_channel_( Guest::pointer guest, const_buffer data ) {
/* ---------------------------------------------------------------------------------- */
	error_code 		ec;
	const char *	channel;

	channel = boost::asio::buffer_cast<const char *>( data );
	{
		scoped_lock lk( channels_m_ );
		//auto result = channels_.find( channel );

		int channel_idx = -1;
		for (int i = 0; i < channels_.size(); i++)
		{
			std::cout << channels_[i].first << " / " << channel << std::endl;
			
			// This cleans the bits somehow?
			std::cout << strlen(channels_[i].first) << " / " << strlen(channel) << std::endl;
			if ( std::strcmp( channels_[i].first, channel ) == 0 )
			{
				
				channel_idx = i;
				/*
				ec.assign( boost::system::errc::file_exists, proc_errc_ );
				return ec;
				*/
			}
		}

		if (channel_idx == -1)
		{	
			std::cout << "HELP!" << std::endl;
			ec.assign( boost::system::errc::file_exists, proc_errc_ );
				return ec;
		}

		/*
		if ( result == channels_.end() ) {
			ec.assign( boost::system::errc::no_such_file_or_directory, proc_errc_ );
			return ec;
		}
		*/
		else
		{
			auto new_session = boost::make_shared<Session>( guest, std::move( socket_ ), port_, channels_[ channel_idx ].second );
			std::cout << "JOINED!! " << channel << std::endl;
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

	if ( ! guest->has_alias() ) {
		ec.assign( boost::system::errc::success, proc_errc_ );
		return ec;
	}
	/* Guest logged in, find alias in guests_ and remove it */
	{
		scoped_lock lk( guests_m_ );

		for ( auto guest: guests_ ) {
			std::cerr << guest.first << std::endl;
		}
		auto it = guests_.begin();
		while ( it != guests_.end() ) {
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
			boost::this_thread::sleep( boost::posix_time::millisec( 10 ));
			op.run();
		}
	} catch ( boost::thread_interrupted& e ) {
		std::cerr << "Processor: thread " << boost::this_thread::get_id() << " terminating." << std::endl;
	}
}



