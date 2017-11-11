#include "channel.hpp"

Channel::Channel( void ) { }

Channel::~Channel( void ) { }

void Channel::join( shared_ptr<Session> session ) { 
	connections_.insert( session );
	for ( auto msg: log_ )
		session->do_write( msg );
}

void Channel::leave( shared_ptr<Session> session ) {
	connections_.erase( session );
}

void Channel::deliver( Messages msg ) {

	/* Deliver message to all users in channel */
	for ( auto connection: connections_ )
		connection->deliver( msg );

	/* Add message to chat log */
	log_.push_back( msg );

	/* Log will only hold Channel::log_max messages */
	if ( log_.size() > Channel::log_max )
		log_.pop_front();
}

void Channel::process_( Messages msg ) {
	//
}

