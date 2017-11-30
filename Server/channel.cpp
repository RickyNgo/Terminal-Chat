#include "channel.hpp"


Channel::Channel( const char * name ) {
	std::memset( name_, '\0', sizeof( name_ ));
	std::strcpy( name_, name );
}

const char * Channel::name( void ) const {
	return name_;
}

Channel::~Channel( void ) { }

void Channel::join( boost::shared_ptr<Session> session ) { 
	ChannelUtility deliverer;
	connections_.insert( session );
	{
		boost::recursive_mutex::scoped_lock lock( log_m_ );
		for ( auto msg: log_ ) {
			deliverer.send_msg( session, msg );
		}
	}
}

void Channel::leave( boost::shared_ptr<Session> session ) {
	connections_.erase( session );
}

void Channel::deliver( Messages msg ) {

	ChannelUtility deliverer;

	/* Add message to chat log */
	{
		boost::recursive_mutex::scoped_lock lock( log_m_ );
		log_.push_back( msg );
		if ( log_.size() > Channel::log_max )
			log_.pop_front();
	}

	/* Deliver message to all users in channel */
	{
		boost::recursive_mutex::scoped_lock lock( connections_m_ );
		for ( auto connection: connections_ )
			deliverer.send_msg( connection, msg );
	}

}


