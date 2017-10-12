#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>

using boost::asio::ip::tcp;

class Connection :
public boost::enable_shared_from_this<Connection>
{
public:
	void start( void );

	Connection( tcp::socket );
	~Connection( void );

private:
	void do_read( void );
	void on_read( const boost::system::error_code, size_t );

	void do_write( const std::string & );
	void on_write( const boost::system::error_code, size_t );

	tcp::socket socket_;
	char read_buffer_[ 1024 ];
	char write_buffer_[ 1024 ];

};