#ifndef __PROCESSOR_HPP__
#define __PROCESSOR_HPP__

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
#include <iostream>
#include <queue>
#include <list>
#include <map>
#include "guest.hpp"
// #include "channel.hpp"

class Connection;

typedef boost::system::error_code						error_code;

class Processor :
private boost::noncopyable {
public:

	typedef boost::asio::mutable_buffers_1			mutable_buffer;
	typedef boost::function<error_code( void )> 	do_async_op;
	typedef boost::function<void( error_code )> 	on_async_op;
	typedef boost::condition_variable				condition;
	typedef std::pair<const unsigned char *,
		Guest::pointer>								guest;

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

	Processor( io_service & ios );
	~Processor( void );

	/* Commands */

	void 	async_stage( Guest::pointer, on_async_op );
	void	async_login( mutable_buffer, on_async_op );

private:
	io_service &										ios_;
	std::vector<Guest::pointer>							stage_;
	boost::recursive_mutex								stage_m_;
	std::map<const unsigned char *, Guest::pointer>		guests_;
	// std::set<Channel::pointer>						channels_;
	std::queue<Processor::Operation> 					ops_;
	boost::mutex 										ops_m_;
	condition 											cond_;
	boost::thread_group									threads_;
	const std::uint8_t  								num_t_;					

	void			add_( do_async_op, on_async_op );
	void 			run_( void );

	error_code		do_login_( mutable_buffer );
	error_code		do_stage_( Guest::pointer );

};

#endif  /* End Processor */
