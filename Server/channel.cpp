#include "channel.hpp"


Channel::Channel( void ) { }

Channel::~Channel( void ) { }

void Channel::join( boost::shared_ptr<Session> session ) { 
	connections_.insert( session );
}

void Channel::leave( boost::shared_ptr<Session> session ) {
	connections_.erase( session );
}

void Channel::deliver( Messages msg ) {

	ChannelUtility deliverer;

	/* Deliver message to all users in channel */
	for ( auto connection: connections_ )
		deliverer.send_msg(connection, msg);

	/* Add message to chat log */
	log_.push_back( msg );

	/* Log will only hold Channel::log_max messages */
	if ( log_.size() > Channel::log_max )
		log_.pop_front();
}


