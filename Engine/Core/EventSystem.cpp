//---------------------------------------------------------------------------
// EventSystem.cpp
//---------------------------------------------------------------------------

#include "Engine/Core/EventSystem.hpp"

#define STATIC

STATIC EventSystem*		EventSystem::s_instance = nullptr;


//---------------------------------------------------------------------------
STATIC EventSystem* EventSystem::GetInstance()
{
	if ( !s_instance ) s_instance = new EventSystem();

	return s_instance;
}


//---------------------------------------------------------------------------
STATIC void EventSystem::DestroyInstance()
{
	if ( s_instance ) delete s_instance;
	s_instance = nullptr;
}


//---------------------------------------------------------------------------
EventSystem::EventSystem()
{

}


//---------------------------------------------------------------------------
EventSystem::~EventSystem()
{
	DestroyMap( m_eventMap );
	DestroyDeque( m_eventQueue );
}


//---------------------------------------------------------------------------
bool EventSystem::DoesEventExist( const std::string& name )
{
	return m_eventMap.find( StringID( name ) ) != m_eventMap.end();
}


//---------------------------------------------------------------------------
bool EventSystem::DoesEventExist( unsigned int nameID )
{
	return m_eventMap.find( nameID ) != m_eventMap.end();
}


//---------------------------------------------------------------------------
Event* EventSystem::GetEvent( const std::string& name )
{
	if ( !DoesEventExist( name ) ) return nullptr;

	return m_eventMap[ StringID( name ) ]->m_event;
}


//---------------------------------------------------------------------------
void EventSystem::FireEvent( const std::string& name )
{
	if ( !DoesEventExist( name ) ) return;

	unsigned int nameID = StringID( name );
	EventAndListeners* event = m_eventMap[ nameID ];

	for ( Listeners::iterator iter = event->m_eventListeners.begin(); iter != event->m_eventListeners.end(); ++ iter )
	{
		( *iter )->HandleEvent( *event->m_event );
	}
}


//---------------------------------------------------------------------------
bool EventSystem::RegisterEvent( Event* event )
{
	if ( DoesEventExist( event->m_eventNameID ) )
	{
		DebuggerPrintf( "Event system error: event %s already exists!\n", StringValue( event->m_eventNameID ) );
		return false;
	}

	//unsigned int nameID = StringID( name );
	//Event* event = new Event( name );
	m_eventMap.insert( std::make_pair( event->m_eventNameID, new EventAndListeners( event ) ) );
	return true;
}


//---------------------------------------------------------------------------
void EventSystem::RegisterEventListener( const std::string& eventName, void( *handler )( Event& e ) )
{
	if ( !DoesEventExist( eventName ) )
	{
		DebuggerPrintf( "Event system error: Event %s does not exist, cannot attach listener!\n", eventName.c_str() );
		return;
	}

	unsigned int nameID = StringID( eventName );
	AbstractEventListener* listener = new AbstractEventListener( handler );
	m_eventMap[ nameID ]->AddListener( listener );
}


//---------------------------------------------------------------------------
void EventSystem::Update()
{
	//if ( m_eventQueue.empty() ) return;
}