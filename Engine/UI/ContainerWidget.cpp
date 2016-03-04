//---------------------------------------------------------------------------
// ContainerWidget.cpp
//---------------------------------------------------------------------------

#include "Engine/Core/EventSystem.hpp"
#include "Engine/Rendering/Vertex3D.hpp"
#include "Engine/Rendering/VennRenderer.hpp"
#include "Engine/Rendering/RenderFunctions.hpp"
#include "Engine/UI/ContainerWidget.hpp"

extern VennRenderer* g_theRenderer;


//---------------------------------------------------------------------------
ContainerWidget::ContainerWidget()
	: Widget()
	, m_focusWidget( nullptr )
{
	
}


//---------------------------------------------------------------------------
ContainerWidget::ContainerWidget( const Vec2f& screenCoords )
	: Widget( screenCoords )
	, m_focusWidget( nullptr )
{

}


//---------------------------------------------------------------------------
ContainerWidget::ContainerWidget( const Vec2f& screenCoords, const Vec2f& dimensions )
	: Widget( screenCoords, dimensions )
	, m_focusWidget( nullptr )
{

}


//---------------------------------------------------------------------------
ContainerWidget::~ContainerWidget()
{

}


//---------------------------------------------------------------------------
void ContainerWidget::InformOfFocus( Widget* focusWidget )
{
	Widget::InformOfFocus( focusWidget );
	m_focusWidget = focusWidget;
}


//---------------------------------------------------------------------------
void ContainerWidget::InformOfLostFocus( Widget* focusWidget )
{
	Widget::InformOfLostFocus( focusWidget );
	if ( focusWidget == m_focusWidget ) m_focusWidget = nullptr;
}


//---------------------------------------------------------------------------
void ContainerWidget::HandleMousePointerMovement( const Vec2f& mousePosition )
{
	Vec2f transformedPosition = mousePosition;
	m_parentToLocal.ApplyThisMatrixToVector( transformedPosition );

	if ( m_focusWidget )
	{
		m_focusWidget->HandleMousePointerMovement( transformedPosition );
	}

	WidgetChildren::iterator iter;
	for ( iter = m_widgetChildren.begin(); iter != m_widgetChildren.end(); ++ iter )
	{
		if ( *iter == m_focusWidget ) continue;

		( *iter )->HandleMousePointerMovement( transformedPosition );
	}
}


//---------------------------------------------------------------------------
void ContainerWidget::HandleInputEvent( Event& e )
{
	if ( m_focusWidget )
	{
		m_focusWidget->HandleInputEvent( e );
	}

	WidgetChildren::iterator iter;
	for ( iter = m_widgetChildren.begin(); iter != m_widgetChildren.end(); ++ iter )
	{
		if ( *iter == m_focusWidget ) continue;

		( *iter )->HandleInputEvent( e );
	}
}


//---------------------------------------------------------------------------
void ContainerWidget::EnableStencilTest()
{
	Vertex3D verts[ 4 ];
	g_theRenderer->VennColorMask( V_FALSE, V_FALSE, V_FALSE, V_FALSE );
	g_theRenderer->VennEnable( V_STENCIL_TEST );

	g_theRenderer->VennStencilFunc( V_ALWAYS, 1, 1 );
	g_theRenderer->VennStencilOp( V_REPLACE, V_REPLACE, V_REPLACE );

	// render the area of the widget to stencil buffer
	g_theRenderer->PushMatrix();
	g_theRenderer->ApplyTransformation( m_parentToLocal );

	verts[ 0 ].m_position = Vec3f( m_screenPosition.x, m_screenPosition.y, 0.f );
	verts[ 1 ].m_position = Vec3f( m_screenPosition.x + m_dimensions.x, m_screenPosition.y, 0.f );
	verts[ 2 ].m_position = Vec3f( m_screenPosition.x + m_dimensions.x, m_screenPosition.y + m_dimensions.y, 0.f );
	verts[ 3 ].m_position = Vec3f( m_screenPosition.x, m_screenPosition.y + m_dimensions.y, 0.f );
	RenderVertexArray( verts, 4, V_QUADS ); // render to stencil

	g_theRenderer->PopMatrix();

	g_theRenderer->VennColorMask( V_TRUE, V_TRUE, V_TRUE, V_TRUE );
	g_theRenderer->VennStencilFunc( V_NOTEQUAL, 1, 1 );
	g_theRenderer->VennStencilOp( V_KEEP, V_KEEP, V_KEEP );
}


//---------------------------------------------------------------------------
void ContainerWidget::DisableStencilTest()
{
	g_theRenderer->VennDisable( V_STENCIL_TEST );
}