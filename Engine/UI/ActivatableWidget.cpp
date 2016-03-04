//---------------------------------------------------------------------------
// ActivatableWidget.cpp
//---------------------------------------------------------------------------

#include "Engine/Core/EventSystem.hpp"
#include "Engine/Input/InputEvents.hpp"
#include "Engine/Rendering/Material.hpp"
#include "Engine/Rendering/RenderFunctions.hpp"
#include "Engine/UI/ActivatableWidget.hpp"


//---------------------------------------------------------------------------
ActivatableWidget::ActivatableWidget()
	: Widget()
	, m_idleMaterial( nullptr )
	, m_focusedMaterial( nullptr )
	, m_pressedMaterial( nullptr )
	, m_idleTint( Colors::WHITE )
	, m_focusedTint( Colors::WHITE )
	, m_pressedTint( Colors::WHITE )
{
	CreateEventProperties();
}


//---------------------------------------------------------------------------
ActivatableWidget::ActivatableWidget( const Vec2f& screenCoords )
	: Widget( screenCoords )
	, m_idleMaterial( nullptr )
	, m_focusedMaterial( nullptr )
	, m_pressedMaterial( nullptr )
	, m_idleTint( Colors::WHITE )
	, m_focusedTint( Colors::WHITE )
	, m_pressedTint( Colors::WHITE )
{
	CreateEventProperties();
}


//---------------------------------------------------------------------------
ActivatableWidget::ActivatableWidget( const Vec2f& screenCoords, const Vec2f& dimensions )
	: Widget( screenCoords, dimensions )
	, m_idleMaterial( nullptr )
	, m_focusedMaterial( nullptr )
	, m_pressedMaterial( nullptr )
	, m_idleTint( Colors::WHITE )
	, m_focusedTint( Colors::WHITE )
	, m_pressedTint( Colors::WHITE )
{
	CreateEventProperties();
}


//---------------------------------------------------------------------------
ActivatableWidget::ActivatableWidget( const ActivatableWidget& otherWidget )
	: Widget( otherWidget )
	, m_idleMaterial( otherWidget.m_idleMaterial )
	, m_focusedMaterial( otherWidget.m_focusedMaterial )
	, m_pressedMaterial( otherWidget.m_pressedMaterial )
	, m_idleTint( Colors::WHITE )
	, m_focusedTint( Colors::WHITE )
	, m_pressedTint( Colors::WHITE )
{
	CreateEventProperties();
}


//---------------------------------------------------------------------------
ActivatableWidget::~ActivatableWidget()
{

}


//---------------------------------------------------------------------------
void ActivatableWidget::CreateEventProperties()
{
	m_widgetEvents.AddProperty< std::string >( "onActivate", "" );
}


//---------------------------------------------------------------------------
void ActivatableWidget::Activate()
{
	std::string event;
	if ( !m_widgetEvents.GetProperty( "onActivate", event ) ) return;
	EventSystem::GetInstance()->FireEvent( event );
}


//---------------------------------------------------------------------------
void ActivatableWidget::HandleInputEvent( Event& e )
{
	if ( m_widgetState != WIDGET_IDLE )
	{
		unsigned int mouseDownID = StringID( "mouseDown" );
		unsigned int mouseUpID = StringID( "mouseUp" );
		MouseButton buttonID = MOUSE_LBUTTON;

		if ( e.m_eventNameID == mouseDownID && m_widgetState == WIDGET_FOCUSED )
		{
			if ( e.GetProperty< MouseButton >( "button", buttonID ) && buttonID == MOUSE_LBUTTON )
			{
				ChangeState( WIDGET_PRESSED );
			}
		}
		else if ( e.m_eventNameID == mouseUpID && m_widgetState == WIDGET_PRESSED )
		{
			if ( e.GetProperty< MouseButton >( "button", buttonID ) && buttonID == MOUSE_LBUTTON )
			{
				ChangeState( WIDGET_FOCUSED );
				Activate();
			}
		}
	}

	Widget::HandleInputEvent( e );
}


//---------------------------------------------------------------------------
void ActivatableWidget::SetOnActivateEvent( const std::string& eventName )
{
	m_widgetEvents.ChangeProperty( "onActivate", eventName );
}


//---------------------------------------------------------------------------
void ActivatableWidget::SetMaterials( const std::string& idle, const std::string& focused, const std::string& pressed )
{
	SetIdleMaterial( idle );
	SetFocusedMaterial( focused );
	SetPressedMaterial( pressed );
}


//---------------------------------------------------------------------------
void ActivatableWidget::SetMaterials( Material* idle, Material* focused, Material* pressed )
{
	m_idleMaterial = idle;
	m_focusedMaterial = focused;
	m_pressedMaterial = pressed;
}


//---------------------------------------------------------------------------
void ActivatableWidget::SetTints( const Rgba& idle, const Rgba& focused, const Rgba& pressed )
{
	SetIdleTint( idle );
	SetFocusedTint( focused );
	SetPressedTint( pressed );
}


//---------------------------------------------------------------------------
void ActivatableWidget::SetIdleMaterial( const std::string& name )
{
	m_idleMaterial = Material::GetMaterial( name );
}


//---------------------------------------------------------------------------
void ActivatableWidget::SetFocusedMaterial( const std::string& name )
{
	m_focusedMaterial = Material::GetMaterial( name );
}


//---------------------------------------------------------------------------
void ActivatableWidget::SetPressedMaterial( const std::string& name )
{
	m_pressedMaterial = Material::GetMaterial( name );
}


//---------------------------------------------------------------------------
void ActivatableWidget::Render()
{
	if ( m_widgetState == WIDGET_HIDDEN ) return;

	Material::GetDefaultUnlitMaterial()->ActivateMaterial();
	g_theRenderer->PushMatrix();
	g_theRenderer->ApplyTransformation( m_parentToLocal );
	g_theRenderer->ApplyTranslation( Vec3f( m_screenPosition.x, m_screenPosition.y, 0.f ) );
	g_theRenderer->ApplyScaling( Vec3f( m_dimensions.x, m_dimensions.y, 0.f ) );

	switch ( m_widgetState )
	{
		default:
		case WIDGET_IDLE:
		{
			RenderIdle();
			break;
		}
		case WIDGET_FOCUSED:
		{
			RenderFocused();
			break;
		}
		case WIDGET_PRESSED:
		{
			RenderPressed();
			break;
		}
	}

	g_theRenderer->PopMatrix();

	g_theRenderer->PushMatrix();
	g_theRenderer->ApplyTransformation( m_parentToLocal );
	for ( WidgetChildren::iterator iter = m_widgetChildren.begin(); iter != m_widgetChildren.end(); ++ iter )
	{
		( *iter )->Render();
	}
	g_theRenderer->PopMatrix();
}


//---------------------------------------------------------------------------
void ActivatableWidget::RenderIdle()
{
	if ( !m_idleMaterial ) return;

	m_idleMaterial->ActivateMaterial();
	RenderTexturedQuadWithVertexArrayAndShader( m_idleTint, m_idleMaterial->GetShaderProgramID(), V_QUADS );
}


//---------------------------------------------------------------------------
void ActivatableWidget::RenderFocused()
{
	if ( !m_focusedMaterial ) return;

	m_focusedMaterial->ActivateMaterial();
	RenderTexturedQuadWithVertexArrayAndShader( m_focusedTint, m_focusedMaterial->GetShaderProgramID(), V_QUADS );
}


//---------------------------------------------------------------------------
void ActivatableWidget::RenderPressed()
{
	if ( !m_pressedMaterial ) return;

	m_pressedMaterial->ActivateMaterial();
	RenderTexturedQuadWithVertexArrayAndShader( m_pressedTint, m_pressedMaterial->GetShaderProgramID(), V_QUADS );
}