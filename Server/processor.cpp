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
/*            Commands            */
/* ------------------------------ */

void Processor::async_stage( Guest::pointer guest, on_async_op comp ) {
	do_async_op fn = boost::bind( &Processor::do_stage_, this, guest );
	ios_.post( boost::bind( &Processor::add_, this, fn, comp ));
}

void Processor::async_login( mutable_buffer buff, on_async_op comp ) {
	do_async_op fn = boost::bind( &Processor::do_login_, this, buff );
	ios_.post( boost::bind( &Processor::add_, this, fn, comp ));
}

/* ----------------------------------- */
/* Internal Processor Request Helpers  */
/* ----------------------------------- */

error_code Processor::do_stage_( Guest::pointer guest ) {
	error_code ec;
	{
		boost::recursive_mutex::scoped_lock lk( stage_m_ );
		stage_.push_back( guest );
	}
	return ec;
}

error_code Processor::do_login_( mutable_buffer data ) {

	std::cerr << "in do_login_" << std::endl;
	error_code ec;
	const unsigned char * alias;
	
	alias = boost::asio::buffer_cast<const unsigned char *>( data );
	std::cerr << "alias: " << alias << std::endl;

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
		cond_.notify_one();
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




