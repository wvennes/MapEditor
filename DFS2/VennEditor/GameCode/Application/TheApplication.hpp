//---------------------------------------------------------------------------
// TheApplication.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_THEAPPLICATION_
#define _INCLUDED_THEAPPLICATION_

class TheEditor;
class TheRenderer;
class TextRenderer;
//class InputManager;
class Event;


/////////////////////////////////////////////////////////////////////////////
class TheApplication
{
public:
	TheApplication( void* platformHandler, void* displayDeviceContext );
	~TheApplication();

	void				Run();

private:
	void*				m_platformHandler;
	void*				m_displayDeviceContext;

	TheEditor*			m_theEditor;

private:
	void				Initialize();
	void				RegisterEventListeners();
	void				RunFrame();
	void				ProcessInput();
// 	void				OnKeyDown( Event& e );
// 	void				OnKeyUp( Event& e );
	void				OnShutdown( Event& e );
	void				Update();
	void				Render();

};

extern TextRenderer*	g_theTextRenderer;
extern VennRenderer*	g_theRenderer;
//extern InputManager*	g_theInputManager;
extern bool				g_isQuitting;
extern bool				g_isGameOver;

#endif