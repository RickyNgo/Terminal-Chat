#ifndef __PROCESSOR_HPP__
#define __PROCESSOR_HPP__

#include <boost/system/error_code.hpp>
using boost::system::error_code;
#include <boost/asio.hpp>
using boost::asio::io_service;
#include <boost/asio/buffer.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/thread.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/condition_variable.hpp>
using boost::condition_variable;
#include <iostream>
#include <string>
#include <queue>
#include <map>
#include "guest.hpp"
#include "channel.hpp" 
#include "session.hpp"

class Connection;

typedef boost::asio::mutable_buffers_1				mutable_buffer;
typedef boost::asio::const_buffers_1				const_buffer;

class Processor :
private boost::noncopyable {
public:

	typedef boost::function<error_code( void )> 		do_async_op;
	typedef boost::function<void( error_code )> 		on_async_op;

	Processor( io_service & ios );
	~Processor( void );

	/* Commands */

	void	async_login( Guest::pointer, const_buffer, on_async_op );
	void	async_leave( Guest::pointer, on_async_op );
	void	async_create_channel( const_buffer, on_async_op );
	void	async_join_channel( const_buffer, on_async_op );
	void	async_close_channel( const_buffer, on_async_op );


private:


	typedef boost::recursive_mutex::scoped_lock			scoped_lock;
	typedef std::pair<const char *, Guest::pointer>		guest_t;
	typedef std::pair<const char *, Channel>			channel_t;
	tcp::socket 									socket_;
	/* Start Error Code Category */

#ifndef __ERROR__
#define __ERROR__

	class Error :
	public boost::system::error_category {
	public:
 		const char * 	name( void ) const noexcept;
  		std::string 	message( int ev ) const;
	};

#endif

/* Start Operation Class */

#ifndef __OPERATION__
#define __OPERATION__

	class Operation :
	public boost::enable_shared_from_this<Processor::Operation> {
	public:
		typedef boost::shared_ptr<Processor::Operation>		pointer;
		Operation( io_service &, do_async_op, on_async_op );
		~Operation( void );

		void 			run( void );
		
	private:

		error_code 					do_operation_( void );
		void						on_operation_( error_code );

		boost::asio::io_service & 	ios_;
		do_async_op					func_;
		on_async_op					complete_;

	};

#endif 

/* End Operation Class */

#ifndef __COMPARATOR_EQ__
#define __COMPARATOR_EQ__

class Equal {
public:
	bool operator() ( const char *, const char * ) const noexcept;
};

#endif

	Error 													proc_errc_;
	io_service &											ios_;
	std::map<const char *, Guest::pointer, Equal>			guests_;
	boost::recursive_mutex									guests_m_;
	std::map<const char *, Channel, Equal>					channels_;
	boost::recursive_mutex									channels_m_;
	std::queue<Processor::Operation> 						ops_;
	boost::mutex 											ops_m_;
	condition_variable 										cond_;
	boost::thread_group										threads_;
	const std::uint8_t  									num_t_;					

	void			add_( do_async_op, on_async_op );
	void 			run_( void );

	/* Command helpers */

	error_code		do_login_( Guest::pointer, const_buffer );
	error_code 		do_create_channel_( const_buffer );
	error_code		do_join_channel_( const_buffer );
	error_code		do_close_channel_( const_buffer );
	error_code		do_leave_( Guest::pointer );
};

#endif  /* End Processor */