#include "connection.hpp"

ConnectionHandler::ConnectionHandler( void ) { }

ConnectionHandler::~ConnectionHandler( void ) { }

void ConnectionHandler::join( Guest::pointer guest ) {
	guests_.push_back( guest );
}

void ConnectionHandler::stop_all( void ) {
	for ( auto guest: guests_ ) {
		guest->quit();
	}
	guests_.clear();
}

void ConnectionHandler::stop( Guest::pointer guest ) {
	guests_.remove( guest );
}

void ConnectionHandler::request( const char * msg, Guest::pointer guest ) {
	if ( strlen( msg ) == 0 ) {
		guest->response( boost::make_shared<Messages>( "Server", "Empty Command", 0, 0 ));
		return;
	} 

	std::string m( msg );
	/* TESTING LOGIN */

	for ( auto g: guests_ ) {
		if ( g->get_alias() == m ) {
				if ( HANDLER_OUT ) std::cerr << "Handler: user " << m << " already exists." << std::endl;
				guest->response( boost::make_shared<Messages>( "Server", "Error: Alias already taken", 0, LOGIN ));
				return;
		}
	}
	std::string a( msg );
	guest->set_alias( a );
	if ( HANDLER_OUT ) std::cerr << "Handler: new user " << m << " added." << std::endl;
	guest->response( boost::make_shared<Messages>( "Server", "OK", 0, LOGIN ));

	// Messages req( msg );
	// std::string req_body = req.get_body();
	// std::cerr << "Connection Request: " << req.get_command() << std::endl;

	// /* LOGIN */
	// if ( req.get_command() == LOGIN ) { 
	// 	for ( auto g: guests_ ) {
	// 		if ( g->get_alias() == req_body ) {
	// 			if ( HANDLER_OUT ) std::cerr << "Handler: user " << req_body << " already exists." << std::endl;
	// 			guest->response( boost::make_shared<Messages>( "Server", "Error: Alias already taken", 0, LOGIN ));
	// 			return;
	// 		}
	// 	}
	// 	if ( HANDLER_OUT ) std::cerr << "Handler: new user " << req_body << " added." << std::endl;
	// 	guest->response( boost::make_shared<Messages>( "Server", "OK", 0, LOGIN ));

	// /* CREATE CHANNEL */
	// } else if ( req.get_command() == CREATE_CHANNEL ) {

	// 	// /* Create Channel */
	// 	// case CREATE_CHANNEL:
	// 	// if ( channels_.find( req_body ) == channels_.end() ) {
	// 	// 	/* Create channel and add user to it */
	// 	// 	channels_.insert( std::pair<std::string, Channel::pointer>( req_body, boost::make_shared<Channel>() ));
	// 	// 	response_( boost::make_shared<Messages>( "Server", "Channel Created.", 0, CREATE_CHANNEL ), c );
	// 	// 	if ( HANDLER_OUT ) std::cerr << "Handler: created new channel " << req_body << "." << std::endl;
	// 	// } else {
	// 	// 	response_( boost::make_shared<Messages>( "Server", "Error: Channel already exists", 0, CREATE_CHANNEL ), c );
	// 	// 	if ( HANDLER_OUT ) std::cerr << "Handler Error: " << req_body << "already exists." << std::endl;

	// 	// }
	// 	// break;

	// 	/* There was an error delivering the command */
	// } else {
	// 	guest->response( boost::make_shared<Messages>( "Server", "Command Error", 0, 0 ));
	// }
}

