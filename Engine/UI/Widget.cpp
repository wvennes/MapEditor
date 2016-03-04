//---------------------------------------------------------------------------
// Widget.cpp
//---------------------------------------------------------------------------

#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/EventSystem.hpp"
//#include "Engine/Rendering/Texture.hpp"
#include "Engine/Rendering/VennRenderer.hpp"
#include "Engine/Rendering/RenderFunctions.hpp"
#include "Engine/Rendering/Material.hpp"
#include "Engine/UI/Widget.hpp"

#define STATIC
#define UNUSED( X ) ( void ) ( x )

STATIC unsigned int	Widget::s_idCounter = 0;

extern VennRenderer* g_theRenderer;


//---------------------------------------------------------------------------
Widget::Widget()
	: m_widgetID( s_idCounter ++ )
	, m_widgetProperties( 0 )
	, m_screenPosition( 0.f, 0.f )
	, m_dimensions( 0.f, 0.f )
	, m_parentToLocal( Matrix4() )
	, m_reverseLocalToParent( Matrix4() )
	, m_cumulativeScreenToLocal( Matrix4() )
	, m_reverseCumulativeLocalToScreen( Matrix4() )
	, m_parentWidget( nullptr )
	, m_widgetState( WIDGET_IDLE )
{
	m_parentToLocal.SetToIdentity();
	m_reverseLocalToParent.SetToIdentity();
	m_cumulativeScreenToLocal.SetToIdentity();
	m_reverseCumulativeLocalToScreen.SetToIdentity();
}


//---------------------------------------------------------------------------
Widget::Widget( const Vec2f& screenCoords )
	: m_widgetID( s_idCounter ++ )
	, m_widgetProperties( 0 )
	, m_screenPosition( screenCoords )
	, m_dimensions( 0.f, 0.f )
	, m_parentToLocal( Matrix4() )
	, m_reverseLocalToParent( Matrix4() )
	, m_cumulativeScreenToLocal( Matrix4() )
	, m_reverseCumulativeLocalToScreen( Matrix4() )
	, m_parentWidget( nullptr )
	, m_widgetState( WIDGET_IDLE )
{
	m_parentToLocal.SetToIdentity();
	m_reverseLocalToParent.SetToIdentity();
	m_cumulativeScreenToLocal.SetToIdentity();
	m_reverseCumulativeLocalToScreen.SetToIdentity();
}


//---------------------------------------------------------------------------
Widget::Widget( const Vec2f& screenCoords, const Vec2f& dimensions )
	: m_widgetID( s_idCounter ++ )
	, m_widgetProperties( 0 )
	, m_screenPosition( screenCoords )
	, m_dimensions( dimensions )
	, m_parentToLocal( Matrix4() )
	, m_reverseLocalToParent( Matrix4() )
	, m_cumulativeScreenToLocal( Matrix4() )
	, m_reverseCumulativeLocalToScreen( Matrix4() )
	, m_parentWidget( nullptr )
	, m_widgetState( WIDGET_IDLE )
{
	m_parentToLocal.SetToIdentity();
	m_reverseLocalToParent.SetToIdentity();
	m_cumulativeScreenToLocal.SetToIdentity();
	m_reverseCumulativeLocalToScreen.SetToIdentity();
}


//---------------------------------------------------------------------------
Widget::Widget( const Widget& otherWidget )
	: m_widgetID( s_idCounter ++ )
	, m_widgetProperties( otherWidget.m_widgetProperties )
	, m_screenPosition( otherWidget.m_screenPosition )
	, m_dimensions( otherWidget.m_dimensions )
	, m_parentToLocal( otherWidget.m_parentToLocal )
	, m_reverseLocalToParent( otherWidget.m_reverseLocalToParent )
	, m_cumulativeScreenToLocal( otherWidget.m_cumulativeScreenToLocal )
	, m_reverseCumulativeLocalToScreen( otherWidget.m_reverseCumulativeLocalToScreen )
	, m_parentWidget( nullptr )
	, m_widgetState( WIDGET_IDLE )
{

}


//---------------------------------------------------------------------------
Widget::~Widget()
{
	DestroyVector( m_widgetChildren );
}


//---------------------------------------------------------------------------
void Widget::HandleMousePointerMovement( const Vec2f& mousePosition )
{
	if ( m_widgetState == WIDGET_HIDDEN || m_widgetState == WIDGET_DISABLED ) return;
	// check if mouse is within the widget dimensions
	// TODO: handle cases where widget is rotated?
	Vec2f transformedPosition = mousePosition;
	Vec2f mins = m_screenPosition;
	Vec2f maxes = m_dimensions + m_screenPosition;

	m_parentToLocal.ApplyThisMatrixToVector( transformedPosition );
	m_parentToLocal.ApplyThisMatrixToVector( mins );
	m_parentToLocal.ApplyThisMatrixToVector( maxes );

	if ( mins.x <= mousePosition.x && mins.y <= mousePosition.y && maxes.x >= mousePosition.x && maxes.y >= mousePosition.y )
	{
		// point is in the box
		SetFocusedAndInformParent();
	}
	else
	{
		SetNotFocusedAndInformParent();
	}

	WidgetChildren::iterator iter;
	for ( iter = m_widgetChildren.begin(); iter != m_widgetChildren.end(); ++ iter )
	{
		( *iter )->HandleMousePointerMovement( transformedPosition );
	}
}


//---------------------------------------------------------------------------
void Widget::HandleInputEvent( Event& e )
{
	if ( m_widgetState == WIDGET_HIDDEN || m_widgetState == WIDGET_DISABLED ) return;

	WidgetChildren::iterator iter;
	for ( iter = m_widgetChildren.begin(); iter != m_widgetChildren.end(); ++ iter )
	{
		( *iter )->HandleInputEvent( e );
	}
}


//---------------------------------------------------------------------------
void Widget::SetFocusedAndInformParent()
{
	if ( m_widgetState != WIDGET_IDLE ) return;

	ChangeState( WIDGET_FOCUSED );
	if ( !m_parentWidget ) return;
	m_parentWidget->InformOfFocus( this );
}


//---------------------------------------------------------------------------
void Widget::SetNotFocusedAndInformParent()
{
	ChangeState( WIDGET_IDLE );
	if ( !m_parentWidget ) return;
	m_parentWidget->InformOfLostFocus( this );
}


//---------------------------------------------------------------------------
void Widget::InformOfFocus( Widget* focusWidget )
{
	if ( !m_parentWidget ) return;

	m_parentWidget->InformOfFocus( focusWidget );
}


//---------------------------------------------------------------------------
void Widget::InformOfLostFocus( Widget* notFocusedWidget )
{
	if ( !m_parentWidget ) return;
	m_parentWidget->InformOfLostFocus( notFocusedWidget );
}


//---------------------------------------------------------------------------
void Widget::AddChildWidget( Widget* widget )
{
	Matrix4 parentToLocal;
	parentToLocal.ApplyTransformationMatrix( Matrix4::GetTranslationMatrix( m_screenPosition ) );
	widget->m_parentToLocal = parentToLocal;
	widget->m_reverseLocalToParent = parentToLocal;
	widget->m_reverseLocalToParent.Invert();

	widget->m_cumulativeScreenToLocal = m_reverseCumulativeLocalToScreen;
	widget->m_cumulativeScreenToLocal.ApplyTransformationMatrix( Matrix4::GetTranslationMatrix( m_screenPosition ) );
	widget->m_reverseCumulativeLocalToScreen = widget->m_reverseCumulativeLocalToScreen;
	widget->m_reverseCumulativeLocalToScreen.Invert();

	m_widgetChildren.push_back( widget );
}


//---------------------------------------------------------------------------
void Widget::Render()
{
	if ( m_widgetState == WIDGET_HIDDEN ) return;

	g_theRenderer->PushMatrix();
	g_theRenderer->ApplyTransformation( m_parentToLocal );
	for ( WidgetChildren::iterator iter = m_widgetChildren.begin(); iter != m_widgetChildren.end(); ++ iter )
	{
		( *iter )->Render();
	}
	g_theRenderer->PopMatrix();
}