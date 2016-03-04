//---------------------------------------------------------------------------
// EventSystem.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_EVENTSYSTEM_
#define _INCLUDED_EVENTSYSTEM_

#include <string>
#include <map>
#include <vector>
#include <deque>

#include "Engine/Core/StringTable.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/Properties.hpp"

class EventAndListeners;
class AbstractEventListener;
class Event;
class AbstractProperty;
typedef std::deque< Event* >							EventQueue;
typedef std::vector< AbstractEventListener* >			Listeners;
typedef std::map< unsigned int, EventAndListeners* >	EventListenerMap;


/////////////////////////////////////////////////////////////////////////////
class Event
{
public:
	Event( const std::string& name ) : m_eventNameID( StringID( name ) ) {}
	Event( Event& otherEvent ) : m_eventNameID( otherEvent.m_eventNameID ), m_properties( otherEvent.m_properties ) {}
	~Event() {}

	template < typename T >
	bool	GetProperty( const std::string& name, T& reference )
	{
		return m_properties.GetProperty< T >( name, reference );
	}

	template < typename T >
	bool	ChangeProperty( const std::string& name, const T& reference )
	{
		return m_properties.ChangeProperty< T >( name, reference );
	}

	template < typename T >
	bool	AddOrChangeProperty( const std::string& name, const T& reference )
	{
		return m_properties.AddOrChangeProperty< T >( name, reference );
	}

	template < typename T >
	bool	AddProperty( const std::string& name, const T& reference )
	{
		return m_properties.AddProperty< T >( name, reference );
	}

public:
	unsigned int	m_eventNameID;
	Properties		m_properties;

};


/////////////////////////////////////////////////////////////////////////////
class AbstractEventListener
{
public:
	AbstractEventListener() : m_nonmemberHandler( nullptr ) {}
	AbstractEventListener( void( *handler )( Event& event ) ) : m_nonmemberHandler( handler ) {}
	virtual void	HandleEvent( Event& e )
	{
		if ( m_nonmemberHandler )
		{
			( *m_nonmemberHandler ) ( e );
		}
	}

protected:
	void( *m_nonmemberHandler )( Event& e );	// event handler for non-member functions

};


/////////////////////////////////////////////////////////////////////////////
template < typename T >
class EventListener : public AbstractEventListener
{
public:

	//---------------------------------------------------------------------------
	EventListener( T* handlerContext, void( T::*handler )( Event& event ) )
		: AbstractEventListener()
		, m_eventContext( handlerContext )
		, m_memberHandler( handler )
	{

	}

	//---------------------------------------------------------------------------
	EventListener( void( *handler )( Event& event ) )
		: AbstractEventListener( handler )
		, m_eventContext( nullptr )
		, m_memberHandler( nullptr )
	{

	}

	~EventListener() {}

	//---------------------------------------------------------------------------
	void HandleEvent( Event& e )
	{
		if ( m_nonmemberHandler )
		{
			( *m_nonmemberHandler) ( e );
		}
		else
		{
			( m_eventContext->*m_memberHandler ) ( e );
		}
	}

	const T* GetContext() const { return m_eventContext; }

private:
	T* m_eventContext;
	void( T::*m_memberHandler ) ( Event& e );	// event handler for member functions
	
};


/////////////////////////////////////////////////////////////////////////////
class EventAndListeners
{
public:
	EventAndListeners() : m_event( nullptr ) {}
	EventAndListeners( Event* event ) : m_event( event ) {}
	~EventAndListeners()
	{
		if ( m_event ) delete m_event;
		DestroyVector( m_eventListeners );
	}

	void			AddListener( AbstractEventListener* listener ) { m_eventListeners.push_back( listener ); }

	Event*			m_event;
	Listeners		m_eventListeners;
};


/////////////////////////////////////////////////////////////////////////////
// EventListenerHandler - this will automatically clean up listeners for a context when destroyed
/////////////////////////////////////////////////////////////////////////////
template < typename T >
class EventListenerCleaner
{
public:
	EventListenerCleaner( T* context ) : m_context( context ) {}
	~EventListenerCleaner()
	{
		std::vector< unsigned int >::iterator iter;
		for ( iter = m_registeredEventNameIDs.begin(); iter != m_registeredEventNameIDs.end(); ++ iter )
		{
			EventSystem::GetInstance()->UnregisterEventListenersForContext< T >( StringValue( *iter ), m_context );
		}
	}

	void AddEvent( const std::string& name ) { m_registeredEventNameIDs.push_back( StringID( name ) ); }

private:
	T*							m_context;
	std::vector< unsigned int >	m_registeredEventNameIDs;

};


/////////////////////////////////////////////////////////////////////////////
class EventSystem
{
public:
	static EventSystem*	GetInstance();
	static void			DestroyInstance();

	bool				DoesEventExist( const std::string& name );
	bool				DoesEventExist( unsigned int nameID );
	Event*				GetEvent( const std::string& name );
	void				FireEvent( const std::string& name );
	bool				RegisterEvent( Event* event );
	void				RegisterEventListener( const std::string& eventName, void( *handler )( Event& e ) );
	void				Update();

	// template functions
	template < typename T >
	void				RegisterEventListener( const std::string& eventName, T* context, void( T::*handler )( Event& e ) );
	template < typename T >
	void				UnregisterEventListenersForContext( const std::string& eventName, T* context );
	//template < typename T >
	//void				UnregisterEventListenersForContext( T* context );
	template < typename T >
	void				FireEventAndUpdateOrAddProperties( const std::string& eventName, const std::string& dataName, const T& data );

private:
	EventListenerMap	m_eventMap;
	EventQueue			m_eventQueue;
	static EventSystem*	s_instance;

private:
	EventSystem();
	~EventSystem();

};


//---------------------------------------------------------------------------
template < typename T >
void EventSystem::RegisterEventListener( const std::string& eventName, T* context, void( T::*handler )( Event& e ) )
{
	if ( !DoesEventExist( eventName ) )
	{
		DebuggerPrintf( "Event system error: event %s doesn't exist, cannot attach listener.\n", eventName.c_str() );
		return;
	}

	unsigned int nameID = StringID( eventName );
	EventListener< T >* listener = new EventListener< T >( context, handler );

	m_eventMap[ nameID ]->AddListener( listener );
}


//---------------------------------------------------------------------------
template < typename T >
void EventSystem::UnregisterEventListenersForContext( const std::string& eventName, T* context )
{
	if ( !DoesEventExist( eventName ) ) return;

	unsigned int nameID = StringID( eventName );
	EventAndListeners* e = m_eventMap[ nameID ];

	for ( Listeners::iterator iter = e->m_eventListeners.begin(); iter != e->m_eventListeners.end(); )
	{
		EventListener< T >* listener = dynamic_cast< EventListener< T >* >( *iter );
		if ( listener )
		{
			if ( listener->GetContext() == context )
			{
				delete listener;
				iter = e->m_eventListeners.erase( iter );
			}
			else
			{
				++ iter;
			}
		}
		else
		{
			++ iter;
		}
	}
}


//---------------------------------------------------------------------------
template < typename T >
void EventSystem::FireEventAndUpdateOrAddProperties( const std::string& eventName, const std::string& dataName, const T& data )
{
	if ( !DoesEventExist( eventName ) ) return;

	unsigned int nameID = StringID( eventName );
	EventAndListeners* event = m_eventMap[ nameID ];

	// update event properties
	if ( !event->m_event->ChangeProperty< T >( dataName, data ) )
	{
		event->m_event->AddProperty< T >( dataName, data );
	}

	// handle the event after updating the data
	for ( Listeners::iterator iter = event->m_eventListeners.begin(); iter != event->m_eventListeners.end(); ++ iter )
	{
		( *iter )->HandleEvent( *event->m_event );
	}
}


#endif