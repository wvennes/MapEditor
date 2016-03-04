//---------------------------------------------------------------------------
// ButtonWidget.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_BUTTONWIDGET_
#define _INCLUDED_BUTTONWIDGET_

#include "Engine/UI/ActivatableWidget.hpp"
#include "Engine/Math/Colors.hpp"

class TextRenderer;

/////////////////////////////////////////////////////////////////////////////
class ButtonWidget : public ActivatableWidget
{
public:
	ButtonWidget();
	ButtonWidget( const Vec2f& screenCoords );
	ButtonWidget( const Vec2f& screenCoords, const Vec2f& dimensions );
	ButtonWidget( const ButtonWidget& otherWidget );
	~ButtonWidget();

	virtual void	Render();
	void			SetLabelText( const char* text, const char* fontMetadataFilepath, float fontSize, const Rgba& color );

protected:
	Vec2f			CalculateRelativePositionForTextLabel( const char* text, float fontSize, const TextRenderer* textRenderer );

};

#endif