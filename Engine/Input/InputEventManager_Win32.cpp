//---------------------------------------------------------------------------
// InputEventManager.cpp
//---------------------------------------------------------------------------

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/EventSystem.hpp"
#include "Engine/Input/InputEventManager.hpp"
#define STATIC

STATIC InputEventManager*	InputEventManager::s_manager = nullptr;


//---------------------------------------------------------------------------
LRESULT CALLBACK InputEventManager_WindowsProcedure( HWND windowHandle, UINT wmMessageCode, WPARAM wParam, LPARAM lParam )
{
	unsigned char asKey = ( unsigned char ) wParam;
	InputEventManager* manager = InputEventManager::GetInstance();
	switch ( wmMessageCode )
	{
		// these are various ways to close a program, we want to pick them up
	case WM_CLOSE:
	case WM_DESTROY:
	case WM_QUIT:
	{
		//g_isQuitting = true;
		//if ( s_manager ) s_manager->SetKeyDownState( KEY_ENGINE_SHUTDOWN );
		// we want this action, return 0
		EventSystem::GetInstance()->FireEvent( "shutdown" );
		return 0;
	}

	case WM_KEYDOWN:
	{
		if ( manager )
		{
			manager->SetKeyDownState( asKey );
			//EventSystem::GetInstance()->FireEventAndUpdateOrAddProperties< unsigned char >( "keyDown", "key", asKey );
		}
		return 0;
	}

	case WM_KEYUP:
	{
		if ( manager )
		{
			manager->SetKeyUpState( asKey );
			//EventSystem::GetInstance()->FireEventAndUpdateOrAddProperties< unsigned char >( "keyUp", "key", asKey );
		}
		return 0;
	}

	case WM_CHAR:
	{

		return 0;
	}

	case WM_LBUTTONDOWN:
	{
		if ( manager )
		{
			manager->SetMouseDownState( MOUSE_LBUTTON );
		}
		return 0;
	}

	case WM_LBUTTONUP:
	{
		if ( manager )
		{
			manager->SetMouseUpState( MOUSE_LBUTTON );
		}
		return 0;
	}

	case WM_MBUTTONDOWN:
	{
		if ( manager )
		{
			manager->SetMouseDownState( MOUSE_MBUTTON );
		}
		return 0;
	}

	case WM_MBUTTONUP:
	{
		if ( manager )
		{
			manager->SetMouseUpState( MOUSE_MBUTTON );
		}
		return 0;
	}

	case WM_RBUTTONDOWN:
	{
		if ( manager )
		{
			manager->SetMouseDownState( MOUSE_RBUTTON );
		}
		return 0;
	}

	case WM_RBUTTONUP:
	{
		if ( manager )
		{
			manager->SetMouseUpState( MOUSE_RBUTTON );
		}
		return 0;
	}

		break;
	}

	// we need to call this to pass the message along if nothing happened that we want
	return DefWindowProc( windowHandle, wmMessageCode, wParam, lParam );
}


//---------------------------------------------------------------------------
InputEventManager::InputEventManager( void* platformHandler )
	: m_platformHandler( platformHandler )
{
	HWND hwnd = ( HWND ) m_platformHandler;
	SetWindowLongPtr( hwnd, GWLP_WNDPROC, ( LONG ) InputEventManager_WindowsProcedure );
}


//---------------------------------------------------------------------------
InputEventManager::~InputEventManager()
{

}


//---------------------------------------------------------------------------
STATIC InputEventManager* InputEventManager::InitializeManager( void* platformHandler )
{
	s_manager = new InputEventManager( platformHandler );

	EventSystem* es = EventSystem::GetInstance();

	// key down event
	Event* keyDown = new Event( "keyDown" );
	keyDown->AddProperty< unsigned char >( "key", 0 );

	// key up event
	Event* keyUp = new Event( "keyUp" );
	keyUp->AddProperty< unsigned char >( "key", 0 );

	// shutdown event
	Event* shutdown = new Event( "shutdown" );

	// mouse events
	Event* mouse = new Event( "mouseEvent" );
	mouse->AddProperty< MouseButton >( "mouse", MOUSE_LBUTTON );

	if ( !es->RegisterEvent( keyDown ) ) delete keyDown;
	if ( !es->RegisterEvent( keyUp ) ) delete keyUp;
	if ( !es->RegisterEvent( shutdown ) ) delete shutdown;
	if ( !es->RegisterEvent( mouse ) ) delete mouse;

	return s_manager;
}


//---------------------------------------------------------------------------
STATIC InputEventManager* InputEventManager::GetInstance()
{
	if ( !s_manager ) DebuggerPrintf( "Warning: input manager not initialized!\n" );
	return s_manager;
}


//---------------------------------------------------------------------------
STATIC void InputEventManager::DestroyInstance()
{
	if ( s_manager ) delete s_manager;
	s_manager = nullptr;
}


//---------------------------------------------------------------------------
void InputEventManager::SetKeyDownState( const unsigned char input )
{
	KeyState& state = m_keyStates[ input ];

	//state.m_isKeyDown = true;
	state.m_hasKeyBeenPressed = true;
	state.m_hasKeyBeenReleased = false;
}


//---------------------------------------------------------------------------
void InputEventManager::SetKeyUpState( const unsigned char input )
{
	KeyState& state = m_keyStates[ input ];

	//state.m_isKeyDown = false;
	state.m_hasKeyBeenPressed = false;
	state.m_hasKeyBeenReleased = true;
}


//---------------------------------------------------------------------------
void InputEventManager::SetMouseDownState( const MouseButton input )
{
	switch ( input )
	{
		default:
		{
			break;
		}
		case MOUSE_LBUTTON:
		{
			m_mouseState.m_lButton.m_hasKeyBeenPressed = true;
			m_mouseState.m_lButton.m_hasKeyBeenReleased = false;
			break;
		}
		case MOUSE_MBUTTON:
		{
			m_mouseState.m_mButton.m_hasKeyBeenPressed = true;
			m_mouseState.m_mButton.m_hasKeyBeenReleased = false;
			break;
		}
		case MOUSE_RBUTTON:
		{
			m_mouseState.m_rButton.m_hasKeyBeenPressed = true;
			m_mouseState.m_rButton.m_hasKeyBeenReleased = false;
			break;
		}
	}
}


//---------------------------------------------------------------------------
void InputEventManager::SetMouseUpState( const unsigned char input )
{
	switch ( input )
	{
		default:
		{
			break;
		}
		case MOUSE_LBUTTON:
		{
			m_mouseState.m_lButton.m_hasKeyBeenPressed = false;
			m_mouseState.m_lButton.m_hasKeyBeenReleased = true;
			break;
		}
		case MOUSE_MBUTTON:
		{
			m_mouseState.m_mButton.m_hasKeyBeenPressed = false;
			m_mouseState.m_mButton.m_hasKeyBeenReleased = true;
			break;
		}
		case MOUSE_RBUTTON:
		{
			m_mouseState.m_rButton.m_hasKeyBeenPressed = false;
			m_mouseState.m_rButton.m_hasKeyBeenReleased = true;
			break;
		}
	}
}


//---------------------------------------------------------------------------
void InputEventManager::Update()
{
	ResetKeyUpAndKeyDownStates();
	UpdateMouseState();
	RunMessagePump();
}


//-----------------------------------------------------------------------------------------------
void InputEventManager::RunMessagePump()
{
	MSG queuedMessage;
	for ( ;; )
	{
		const BOOL wasMessagePresent = PeekMessage( &queuedMessage, NULL, 0, 0, PM_REMOVE );
		if ( !wasMessagePresent )
		{
			break;
		}

		TranslateMessage( &queuedMessage );
		DispatchMessage( &queuedMessage );
	}
}


//---------------------------------------------------------------------------
void InputEventManager::ResetKeyUpAndKeyDownStates()
{
	for ( int index = 0; index <= 255; ++ index )
	{
		KeyState& state = m_keyStates[ index ];

		ResetKeyState( state );
	}
}


//---------------------------------------------------------------------------
void InputEventManager::UpdateMouseState()
{
	ResetKeyState( m_mouseState.m_lButton );
	ResetKeyState( m_mouseState.m_mButton );
	ResetKeyState( m_mouseState.m_rButton );

	POINT mousePos;
	RECT window;
	GetCursorPos( &mousePos ); // get screen coordinates
	ScreenToClient( ( HWND ) m_platformHandler, &mousePos ); // convert to coordinates in the window
	// flip y coordinates since they're wrong in windows
	if ( GetClientRect( ( HWND ) m_platformHandler, &window ) )
	{
		m_mouseState.m_mousePosition.y = ( float ) ( window.bottom - window.top - mousePos.y );
	}
	else
	{
		m_mouseState.m_mousePosition.y = ( float ) mousePos.y;
	}

	m_mouseState.m_mousePosition.x = ( float ) mousePos.x;
}


//---------------------------------------------------------------------------
Vec2f InputEventManager::GetWindowSize()
{
	RECT window;
	GetClientRect( ( HWND ) m_platformHandler, &window );
	return Vec2f( ( float ) ( window.right - window.left ), ( float ) ( window.bottom - window.top ) );
}


//---------------------------------------------------------------------------
void InputEventManager::ResetKeyState( KeyState& state )
{
	if ( state.m_hasKeyBeenPressed ) state.m_isKeyDown = true;
	else if ( state.m_hasKeyBeenReleased ) state.m_isKeyDown = false;

	state.m_hasKeyBeenPressed = false;
	state.m_hasKeyBeenReleased = false;
}


//---------------------------------------------------------------------------
bool InputEventManager::IsKeyDown( const unsigned char input ) const
{
	return m_keyStates[ input ].m_isKeyDown;
}


//---------------------------------------------------------------------------
bool InputEventManager::HasKeyBeenPressedOnce( const unsigned char input ) const
{
	return ( HasKeyBeenPressed( input ) && !( IsKeyDown( input ) ) );
}


//---------------------------------------------------------------------------
bool InputEventManager::HasKeyBeenPressed( const unsigned char input ) const
{
	return m_keyStates[ input ].m_hasKeyBeenPressed;
}

//---------------------------------------------------------------------------
bool InputEventManager::HasKeyBeenReleased( const unsigned char input ) const
{
	return m_keyStates[ input ].m_hasKeyBeenReleased;
}


//---------------------------------------------------------------------------
bool InputEventManager::IsMouseButtonDown( const unsigned char input ) const
{
	switch ( input )
	{
		default:
		{
			return false;
		}
		case MOUSE_LBUTTON:
		{
			return m_mouseState.m_lButton.m_isKeyDown;
		}
		case MOUSE_MBUTTON:
		{
			return m_mouseState.m_mButton.m_isKeyDown;
		}
		case MOUSE_RBUTTON:
		{
			return m_mouseState.m_rButton.m_isKeyDown;
		}
	}
}


//---------------------------------------------------------------------------
bool InputEventManager::HasMouseButtonBeenPressedOnce( const unsigned char input ) const
{
	return ( HasMouseButtonBeenPressed( input ) && !( IsMouseButtonDown( input ) ) );
}


//---------------------------------------------------------------------------
bool InputEventManager::HasMouseButtonBeenPressed( const unsigned char input ) const
{
	switch ( input )
	{
		default:
		{
			return false;
		}
		case MOUSE_LBUTTON:
		{
			return m_mouseState.m_lButton.m_hasKeyBeenPressed;
		}
		case MOUSE_MBUTTON:
		{
			return m_mouseState.m_mButton.m_hasKeyBeenPressed;
		}
		case MOUSE_RBUTTON:
		{
			return m_mouseState.m_rButton.m_hasKeyBeenPressed;
		}
	}
}


//---------------------------------------------------------------------------
bool InputEventManager::HasMouseButtonBeenReleased( const unsigned char input ) const
{
	switch ( input )
	{
		default:
		{
			return false;
		}
		case MOUSE_LBUTTON:
		{
			return m_mouseState.m_lButton.m_hasKeyBeenReleased;
		}
		case MOUSE_MBUTTON:
		{
			return m_mouseState.m_mButton.m_hasKeyBeenReleased;
		}
		case MOUSE_RBUTTON:
		{
			return m_mouseState.m_rButton.m_hasKeyBeenReleased;
		}
	}
}


//---------------------------------------------------------------------------
int InputEventManager::SetMouseCursorVisibility( bool isVisible )
{
	return ShowCursor( isVisible );
}


//---------------------------------------------------------------------------
Vec2f InputEventManager::GetMouseMovementSinceLastChecked( long centerX, long centerY )
{
	POINT centerCursorPos = { centerX, centerY };
	POINT cursorPos;
	GetCursorPos( &cursorPos );
	SetCursorPos( centerCursorPos.x, centerCursorPos.y );
	Vec2i mouseDeltaInts( cursorPos.x - centerCursorPos.x, cursorPos.y - centerCursorPos.y );
	Vec2f mouseDeltas( ( float ) mouseDeltaInts.x, ( float ) mouseDeltaInts.y );
	return mouseDeltas;
}


//---------------------------------------------------------------------------
Vec2f InputEventManager::GetMouseCursorLocation()
{
	return m_mouseState.m_mousePosition;
}