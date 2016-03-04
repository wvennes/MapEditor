//---------------------------------------------------------------------------
// TextLabelWidget.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_TEXTLABELWIDGET_
#define _INCLUDED_TEXTLABELWIDGET_

#include "Engine/UI/Widget.hpp"
#include "Engine/Math/Colors.hpp"

class TextRenderer;

/////////////////////////////////////////////////////////////////////////////
class TextLabelWidget : public Widget
{
public:
	TextLabelWidget();
	// 	TextLabelWidget( const Vec2f& screenCoords );
	// 	TextLabelWidget( const Vec2f& screenCoords, const Vec2f& dimensions );
	//  TextLabelWidget( const Vec2f& screenCoords, const Vec2f& dimensions, TextRenderer* textRenderer, float fontSize, const Rgba& color, const char* text );
	TextLabelWidget( const Vec2f& screenCoords, const char* fontMetadataFilepath, float fontSize, const Rgba& color, const char* text );
	TextLabelWidget( const TextLabelWidget& otherWidget );
	~TextLabelWidget();

	const TextRenderer*	GetTextRenderer() const { return m_textRenderer; }
	Rgba				GetColor() const { return m_textColor; }
	std::string			GetText() const { return m_text; }
	float				GetFontSize() const { return m_fontSize; }
	void				SetTextRenderer( const char* fontMetadataFilepath );
	void				SetColor( const Rgba& color ) { m_textColor = color; }
	void				SetText( const char* text ) { m_text = text; }
	void				SetFontSize( float fontSize ) { m_fontSize = fontSize; }

	void				Render();

private:
	float				m_fontSize;
	TextRenderer*		m_textRenderer;
	Rgba				m_textColor;
	std::string			m_text;

};

#endif