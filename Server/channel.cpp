#include "channel.hpp"


Channel::Channel( const char * name ) {
	std::memset( name_, '\0', sizeof( name_ ));
	std::strcpy( name_, name );
}

const char * Channel::name( void ) const {
	return name_;
}

Channel::~Channel( void ) { }

void Channel::join( Participant::pointer participant ) { 
	participants_.insert( participant );
	{
		boost::recursive_mutex::scoped_lock lock( log_m_ );
		for ( auto msg: log_ ) {
			participant->deliver( msg );
		}

		time_t current_time;
		std::string people;

		for (auto contacts : participants_)
		{
			people.append(contacts->alias());
			people.append("|");
		}

		Messages test( "CHANNEL", people, time(&current_time), ONLINE );

		deliver(test);
	}
}

void Channel::leave( Participant::pointer participant ) {
	char buffer[ 256 ];
	time_t current_time;

	std::memset( buffer, '\0', sizeof( buffer ));
	std::sprintf( buffer, "%s has left the channel.", participant->alias());
	{
		boost::recursive_mutex::scoped_lock lock( participants_m_ );
		participants_.erase( participant );
	}
	Messages info( name_, buffer, time( &current_time ), MSG );
	{
		boost::recursive_mutex::scoped_lock lock( participants_m_ );
		for ( auto participant: participants_ )
			participant->deliver( info );
	}
	
	std::string people;

	for (auto contacts : participants_)
	{
		people.append(contacts->alias());
		people.append("|");
	}

	Messages test( "CHANNEL", people, time(&current_time), ONLINE );

	deliver(test);


}

void Channel::deliver( Messages & msg ) {
	/* Add message to chat log */
	{
		boost::recursive_mutex::scoped_lock lock( log_m_ );
		log_.push_back( msg );
		if ( log_.size() > Channel::log_max )
			log_.pop_front();
	}

	/* Deliver message to all users in channel */
	{
		boost::recursive_mutex::scoped_lock lock( participants_m_ );
		for ( auto participant: participants_ )
			participant->deliver( msg );
	}

}

// std::set<boost::shared_ptr<Participant>> Channel::get_participants_()
// {
// 	return participants_;
// }

std::set<Participant::pointer> Channel::get_participants_()
{
 	return participants_;
}