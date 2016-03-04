//---------------------------------------------------------------------------
// DraggableContainerWidget.cpp
//---------------------------------------------------------------------------

#include "Engine/Core/EventSystem.hpp"
#include "Engine/UI/DraggableContainerWidget.hpp"

#define UNUSED( x ) ( void ) ( x )


//---------------------------------------------------------------------------
DraggableContainerWidget::DraggableContainerWidget()
	: ContainerWidget()
{

}


//---------------------------------------------------------------------------
DraggableContainerWidget::DraggableContainerWidget( const Vec2f& screenCoords )
	: ContainerWidget( screenCoords )
{

}


//---------------------------------------------------------------------------
DraggableContainerWidget::DraggableContainerWidget( const Vec2f& screenCoords, const Vec2f& dimensions )
	: ContainerWidget( screenCoords, dimensions )
{

}


//---------------------------------------------------------------------------
DraggableContainerWidget::~DraggableContainerWidget()
{

}


//---------------------------------------------------------------------------
void DraggableContainerWidget::HandleMousePointerMovement( const Vec2f& mousePosition )
{
	UNUSED( mousePosition );
}


//---------------------------------------------------------------------------
void DraggableContainerWidget::HandleInputEvent( Event& e )
{
	UNUSED( e );
}