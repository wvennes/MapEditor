//---------------------------------------------------------------------------
// DraggableContainerWidget.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_DRAGGABLECONTAINERWIDGET_
#define _INCLUDED_DRAGGABLECONTAINERWIDGET_

#include "Engine/UI/ContainerWidget.hpp"

/////////////////////////////////////////////////////////////////////////////
class DraggableContainerWidget : public ContainerWidget
{
public:
	DraggableContainerWidget();
	DraggableContainerWidget( const Vec2f& screenCoords );
	DraggableContainerWidget( const Vec2f& screenCoords, const Vec2f& dimensions );
	~DraggableContainerWidget();

	virtual void	HandleMousePointerMovement( const Vec2f& mousePosition );
	virtual void	HandleInputEvent( Event& e );

protected:

};

#endif