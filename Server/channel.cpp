#include "channel.hpp"

Channel::Channel( void ) { }

Channel::~Channel( void ) { }

// void Channel::join( shared_ptr<Connection> connection ) { 
// 	connections_.insert( connection );
// 	for ( auto msg: log_ )
// 		connection->do_write( msg );
// }

// void Channel::leave( shared_ptr<Connection> connection ) {
// 	connections_.erase( connection );
// }

// void Channel::deliver( Messages msg ) {

// 	 Deliver message to all users in channel 
// 	for ( auto connection: connections_ )
// 		connection->deliver( msg );

// 	/* Add message to chat log */
// 	log_.push_back( msg );

// 	/* Log will only hold Channel::log_max messages */
// 	if ( log_.size() > Channel::log_max )
// 		log_.pop_front();
// }

// void Channel::process_( Messages msg ) {
// 	//
// }

