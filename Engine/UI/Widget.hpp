//---------------------------------------------------------------------------
// Widget.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_WIDGET_
#define _INCLUDED_WIDGET_

//#include "Engine/Core/EventSystem.hpp"
#include <vector>
#include <string>

#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/Properties.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Matrix4.hpp"

#define WidgetBit( x ) ( 1 << x )

class Material;
class Widget;
class Event;

typedef std::vector< std::string >	EventVector;
typedef std::vector< Widget* >		WidgetChildren;


/////////////////////////////////////////////////////////////////////////////
enum WidgetProperties
{
	WP_MOVABLE		= WidgetBit( 0 ),
	WP_CLICKABLE	= WidgetBit( 1 ),
	WP_SCROLLABLE	= WidgetBit( 2 ),
};


/////////////////////////////////////////////////////////////////////////////
enum WidgetState
{
	WIDGET_IDLE,
	WIDGET_FOCUSED,
	WIDGET_PRESSED,
	WIDGET_RELEASED,
// 	WIDGET_SCROLLED,
// 	WIDGET_CLICKED,
	WIDGET_DRAGGED,
	WIDGET_DISABLED,
	WIDGET_HIDDEN,
	NUM_WIDGET_STATES
};


///////////////////////////////////////////////////////////////////////////////
//struct WidgetEvents
//{
//	WidgetEvents() : m_clickedEvent( "" ), m_releasedEvent( "" ) {}
//	~WidgetEvents() {}
//
//	std::string		m_clickedEvent;
//	std::string		m_releasedEvent;
//};


/////////////////////////////////////////////////////////////////////////////
class Widget
{
public:
	Widget();
	Widget( const Vec2f& screenCoords );
	Widget( const Vec2f& screenCoords, const Vec2f& dimensions );
	Widget( const Widget& otherWidget );
	virtual ~Widget();

	virtual void		Render();

	virtual void		SetFocusedAndInformParent();
	virtual void		SetNotFocusedAndInformParent();
	virtual void		InformOfFocus( Widget* focusWidget );
	virtual void		InformOfLostFocus( Widget* notFocusedWidget );
	virtual void		HandleStateChange() {}
	//virtual void		HandleFocus() {}
	//virtual void		HandleUnfocus() {}

	virtual void		HandleMousePointerMovement( const Vec2f& mousePosition );
	//virtual void		HandleKeyboardInput( const unsigned char input );
	//virtual void		HandleMouseInput( const unsigned char input );
	//virtual void		HandleInputEvents();
	virtual void		HandleInputEvent( Event& e );

	Vec2f				GetScreenPosition() const { return m_screenPosition; }
	Vec2f				GetDimensions() const { return m_dimensions; }
	WidgetState			GetWidgetState() const { return m_widgetState; }
	void				SetScreenPosition( const Vec2f& screenPosition ) { m_screenPosition = screenPosition; }
	void				SetDimensions( const Vec2f& dimensions ) { m_dimensions = dimensions; }

// 	void				SetIdleTexture( const std::string& filePath );
// 	void				SetFocusedTexture( const std::string& filePath );
// 	void				SetPressedTexture( const std::string& filePath );
	void				ChangeState( WidgetState state ) { m_widgetState = state; HandleStateChange(); }
	void				AddChildWidget( Widget* widget );

	// widget properties
	void				AddProperty( WidgetProperties property ) { m_widgetProperties |= property; }
	void				RemoveProperty( WidgetProperties property ) { m_widgetProperties &= ~property; }
	bool				DoesHaveProperty( WidgetProperties property ) { return ( ( m_widgetProperties & ( unsigned int ) property ) == ( unsigned int ) property ); }

protected:
	static unsigned int	s_idCounter;

	unsigned int		m_widgetID;
	unsigned int		m_widgetProperties;
	Vec2f				m_screenPosition; // coordinates are at bottom left corner of widget
	Vec2f				m_dimensions;
	Matrix4				m_parentToLocal;
	Matrix4				m_reverseLocalToParent;
	Matrix4				m_cumulativeScreenToLocal;
	Matrix4				m_reverseCumulativeLocalToScreen;
	Widget*				m_parentWidget;
	WidgetState			m_widgetState;
	WidgetChildren		m_widgetChildren;
	Properties			m_widgetEvents;

protected:

};


//---------------------------------------------------------------------------
// template < typename T >
// bool Widget::BindOnClicked( T* context, void( T::*handler )( Event& e ) )
// {
// 	std::string widgetName( StringValue( m_widgetNameID ) );
// 	std::string eventName = "OnClicked_" + widgetName;
// 
// 	return BindToEvent( eventName, context, handler );
// }
// 
// 
// //---------------------------------------------------------------------------
// template < typename T >
// bool Widget::BindOnReleased( T* context, void( T::*handler ) ( Event& e ) )
// {
// 	std::string widgetName( StringValue( m_widgetNameID ) );
// 	std::string eventName = "OnReleased_" + widgetName;
// 
// 	return BindToEvent( eventName, context, handler );
// }
// 
// 
// //---------------------------------------------------------------------------
// template < typename T >
// bool Widget::BindToEvent( const std::string& eventName, T* context, void( T::*handler )( Event& e ) )
// {
// 	EventSystem* es = EventSystem::GetInstance();
// 
// 	if ( es->DoesEventExist( eventName ) ) return false;
// 
// 	Event* e = new Event( eventName );
// 	es->RegisterEvent( e );
// 	es->RegisterEventListener< T >( eventName, context, handler );
// 	return true;
// }

#endif