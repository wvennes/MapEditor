//---------------------------------------------------------------------------
// ButtonWidget.cpp
//---------------------------------------------------------------------------

#include "Engine/Rendering/TextRenderer.hpp"
#include "Engine/Rendering/VennRenderer.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/UI/TextLabelWidget.hpp"
#include "Engine/UI/ButtonWidget.hpp"

extern VennRenderer* g_theRenderer;


//---------------------------------------------------------------------------
ButtonWidget::ButtonWidget()
	: ActivatableWidget()
{

}


//---------------------------------------------------------------------------
ButtonWidget::ButtonWidget( const Vec2f& screenCoords )
	: ActivatableWidget( screenCoords )
{

}


//---------------------------------------------------------------------------
ButtonWidget::ButtonWidget( const Vec2f& screenCoords, const Vec2f& dimensions )
	: ActivatableWidget( screenCoords, dimensions )
{

}


//---------------------------------------------------------------------------
ButtonWidget::ButtonWidget( const ButtonWidget& otherWidget )
	: ActivatableWidget( otherWidget )
{

}


//---------------------------------------------------------------------------
ButtonWidget::~ButtonWidget()
{

}


//---------------------------------------------------------------------------
void ButtonWidget::SetLabelText( const char* text, const char* fontMetadataFilepath, float fontSize, const Rgba& color )
{
	TextLabelWidget* widget = new TextLabelWidget();
	widget->SetTextRenderer( fontMetadataFilepath );

	if ( !widget->GetTextRenderer() )
	{
		delete widget;
		return;
	}

	Vec2f labelPosition = CalculateRelativePositionForTextLabel( text, fontSize, widget->GetTextRenderer() );
	widget->SetScreenPosition( labelPosition );
	widget->SetText( text );
	widget->SetFontSize( fontSize );
	widget->SetColor( color );

	DestroyVector( m_widgetChildren );
	AddChildWidget( widget );
}


//---------------------------------------------------------------------------
Vec2f ButtonWidget::CalculateRelativePositionForTextLabel( const char* text, float fontSize, const TextRenderer* textRenderer )
{
	if ( !textRenderer ) return Vec2f( 0.f, 0.f );
	float textLength = textRenderer->CalculateTextWidth( text, fontSize );

	float offsetX = 0.f + ( m_dimensions.x * 0.5f ) - ( textLength * 0.5f );
	float offsetY = 0.f + ( m_dimensions.y * 0.5f ) - ( fontSize * 0.5f );
	return Vec2f( offsetX, offsetY );
}


//---------------------------------------------------------------------------
void ButtonWidget::Render()
{
	if ( m_widgetState == WIDGET_HIDDEN ) return;

	//Material::GetDefaultUnlitMaterial()->ActivateMaterial();
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

	// render labels
	g_theRenderer->PushMatrix();
	g_theRenderer->ApplyTransformation( m_parentToLocal );
	g_theRenderer->ApplyTranslation( Vec3f( m_screenPosition.x, m_screenPosition.y, 0.f ) );
	for ( WidgetChildren::iterator iter = m_widgetChildren.begin(); iter != m_widgetChildren.end(); ++ iter )
	{
		( *iter )->Render();
	}
	g_theRenderer->PopMatrix();
}