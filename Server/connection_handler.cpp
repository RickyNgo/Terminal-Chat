#include "connection.hpp"

ConnectionHandler::ConnectionHandler( void ) { }

ConnectionHandler::~ConnectionHandler( void ) { }

void ConnectionHandler::connect( Connection::pointer c ) {
	connections_.insert( c );
}

void ConnectionHandler::stop_all( void ) {
	for ( auto connection: connections_ ) 
		connection->stop();
}

void ConnectionHandler::request( boost::shared_ptr<Messages> req, Connection::pointer c ) {
	std::string req_body = req->get_body();
	switch ( req->get_command() ) {

		/* Login */
		case LOGIN:
		if ( users_.find( req->get_body()) == users_.end() ) {
			users_.insert( std::pair<std::string, unsigned long>( req->get_body(), c->get_id()));
			response_( boost::make_shared<Messages>( "Server", "Error: Alias already taken", 0, LOGIN ), c );
			if ( HANDLER_OUT ) std::cerr << "Handler: new user " << req_body << " added." << std::endl;
		} else {
			response_( boost::make_shared<Messages>( "Server", "OK", 0, LOGIN ), c );
			if ( HANDLER_OUT ) std::cerr << "Handler Error: Alias " << req_body << " already exists." << std::endl;
		}
		break;

		/* Create Channel */
		case CREATE_CHANNEL:
		if ( channels_.find( req_body ) == channels_.end() ) {
			/* Create channel and add user to it */
			channels_.insert( std::pair<std::string, Channel::pointer>( req_body, boost::make_shared<Channel>() ));
			response_( boost::make_shared<Messages>( "Server", "Channel Created.", 0, CREATE_CHANNEL ), c );
			if ( HANDLER_OUT ) std::cerr << "Handler: created new channel " << req_body << "." << std::endl;
		} else {
			response_( boost::make_shared<Messages>( "Server", "Error: Channel already exists", 0, CREATE_CHANNEL ), c );
			if ( HANDLER_OUT ) std::cerr << "Handler Error: " << req_body << "already exists." << std::endl;

		}
		break;

		/* There was an error delivering the command */
		default:
		response_( boost::make_shared<Messages>( "Server", "Command Error", 0, 0 ), c );
		break;
	}
}

void ConnectionHandler::response_( boost::shared_ptr<Messages> resp, Connection::pointer c ) { }

