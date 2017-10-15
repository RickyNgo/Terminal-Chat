#include <channel.hpp>

Channel::Channel( std::string title ) : title_( title ) { }

Channel::~Channel( void ) { }

void Channel::join_( User::pointer user ) { 
	users_.insert( User::pointer user );
	for ( auto msg: this->log_ )
		user->deliver( msg );
}

void Channel::leave_( User::pointer user_ ) {
	users_.erase( user_ );
}

void Channel::deliver_( Messages msg ) {

	/* Deliver message to all users in channel */
	for ( auto user: this->users_ )
		user->deliver( msg );

	/* Add message to chat log */
	log_.push_back( msg );

	/* Log will only hold Channel::log_max messages */
	if ( log.size > Channel::log_max )
		log_.pop_front();
}

void Channel::process_( Messages msg ) {
	//
}

