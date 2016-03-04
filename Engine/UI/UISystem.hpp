//---------------------------------------------------------------------------
// UISystem.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_UISYSTEM_
#define _INCLUDED_UISYSTEM_

class Widget;
class ContainerWidget;

/////////////////////////////////////////////////////////////////////////////
class UISystem
{
public:
	static UISystem*	GetInstance();
	static void			DestroyInstance();

	void				Update();
	void				RenderWidgets();

	void				AddToWindow( Widget* widget );

private:
	static UISystem*	s_instance;

	ContainerWidget*	m_rootWindow;

private:
	UISystem();
	~UISystem();

	void				Initialize();

};

#endif