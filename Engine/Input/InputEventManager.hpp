//---------------------------------------------------------------------------
// InputEventManager.hpp
//---------------------------------------------------------------------------


#pragma once
#ifndef _INCLUDED_INPUTEVENTMANAGER_
#define _INCLUDED_INPUTEVENTMANAGER_

#include "Engine/Input/InputEvents.hpp"
#include "Engine/Math/Vec2.hpp"


/////////////////////////////////////////////////////////////////////////////
struct KeyState
{
	bool	m_isKeyDown;
	bool	m_hasKeyBeenPressed;
	bool	m_hasKeyBeenReleased;

	KeyState() : m_isKeyDown( false ), m_hasKeyBeenPressed( false ), m_hasKeyBeenReleased( false ) {}
};


/////////////////////////////////////////////////////////////////////////////
struct MouseState
{
	Vec2f		m_mousePosition;
	KeyState	m_lButton;
	KeyState	m_mButton;
	KeyState	m_rButton;

	MouseState() : m_mousePosition( 0.f, 0.f ), m_lButton( KeyState() ), m_mButton( KeyState() ), m_rButton( KeyState() ) {}
};


/////////////////////////////////////////////////////////////////////////////
class InputEventManager
{
public:
	static InputEventManager*	InitializeManager( void* platformHandler );
	static InputEventManager*	GetInstance();
	static void					DestroyInstance();

	void						Update();

	// keyboard
	void						SetKeyDownState( const unsigned char input );
	void						SetKeyUpState( const unsigned char input );
	bool						IsKeyDown( const unsigned char input ) const;
	bool						HasKeyBeenPressedOnce( const unsigned char input ) const;
	bool						HasKeyBeenPressed( const unsigned char input ) const;
	bool						HasKeyBeenReleased( const unsigned char input ) const;

	// mouse
	void						SetMouseDownState( const MouseButton input );
	void						SetMouseUpState( const unsigned char input );
	bool						IsMouseButtonDown( const unsigned char input ) const;
	bool						HasMouseButtonBeenPressedOnce( const unsigned char input ) const;
	bool						HasMouseButtonBeenPressed( const unsigned char input ) const;
	bool						HasMouseButtonBeenReleased( const unsigned char input ) const;
	int							SetMouseCursorVisibility( bool isVisible );
	Vec2f						GetMouseMovementSinceLastChecked( long centerX, long centerY );
	Vec2f						GetMouseCursorLocation();
	Vec2f						GetWindowSize();

private:
	static InputEventManager*	s_manager;

	void*						m_platformHandler;
	MouseState					m_mouseState;
	KeyState					m_keyStates[ 256 ];

private:
	InputEventManager( void* platformHandler );
	~InputEventManager();
	void						RunMessagePump();
	void						ResetKeyUpAndKeyDownStates();
	void						ResetKeyState( KeyState& state );
	void						UpdateMouseState();

};

#endif