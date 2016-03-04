//---------------------------------------------------------------------------
// TextLabelWidget.cpp
//---------------------------------------------------------------------------

#include "Engine/Rendering/Material.hpp"
#include "Engine/Rendering/TextRenderer.hpp"
#include "Engine/UI/TextLabelWidget.hpp"


//---------------------------------------------------------------------------
TextLabelWidget::TextLabelWidget()
	: Widget()
	, m_fontSize( 0.f )
	, m_textRenderer( nullptr )
	, m_textColor( Colors::WHITE )
	, m_text( "" )
{

}


//---------------------------------------------------------------------------
// TextLabelWidget::TextLabelWidget( const Vec2f& screenCoords, const Vec2f& dimensions, TextRenderer* textRenderer, float fontSize, const Rgba& color, const char* text )
// 	: Widget( screenCoords, dimensions )
// 	, m_fontSize( fontSize )
// 	, m_textRenderer( textRenderer )
// 	, m_textColor( color )
// 	, m_text( text )
// {
// 
// }


//---------------------------------------------------------------------------
TextLabelWidget::TextLabelWidget( const Vec2f& screenCoords, const char* fontMetadataFilepath, float fontSize, const Rgba& color, const char* text )
	: Widget( screenCoords )
	, m_fontSize( fontSize )
	, m_textRenderer( TextRenderer::CreateAndGetTextRenderer( fontMetadataFilepath ) )
	, m_textColor( color )
	, m_text( text )
{

}


//---------------------------------------------------------------------------
TextLabelWidget::~TextLabelWidget()
{
	if ( m_textRenderer ) delete m_textRenderer;
}


//---------------------------------------------------------------------------
void TextLabelWidget::SetTextRenderer( const char* fontMetadataFilepath )
{
	m_textRenderer = TextRenderer::CreateAndGetTextRenderer( fontMetadataFilepath );
}


//---------------------------------------------------------------------------
void TextLabelWidget::Render()
{
	if ( !m_textRenderer ) return;

	m_textRenderer->TextRendererPrintf( m_fontSize, m_screenPosition, m_textColor, Material::GetBoundShaderProgramID(), m_text.c_str() );
}