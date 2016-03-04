//---------------------------------------------------------------------------
// ContainerWidget.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_CONTAINERWIDGET_
#define _INCLUDED_CONTAINERWIDGET_

#include "Engine/UI/Widget.hpp"

/////////////////////////////////////////////////////////////////////////////
class ContainerWidget : public Widget
{
public:
	ContainerWidget();
	ContainerWidget( const Vec2f& screenCoords );
	ContainerWidget( const Vec2f& screenCoords, const Vec2f& dimensions );
	~ContainerWidget();

	virtual void	InformOfFocus( Widget* focusWidget );
	virtual void	InformOfLostFocus( Widget* focusWidget );
	virtual void	HandleMousePointerMovement( const Vec2f& mousePosition );
	virtual void	HandleInputEvent( Event& e );

	//virtual void	Render();
	//virtual void	HandleKeyboardInput( const unsigned char input );
	//virtual void	HandleMouseInput( const unsigned char input );
	//virtual void	HandleInputEvent( Event& e );

protected:
	Widget*			m_focusWidget;

protected:
	void			EnableStencilTest();
	void			DisableStencilTest();

};

#endif