//---------------------------------------------------------------------------
// SpriteSheetButton.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_SPRITESHEETBUTTON_
#define _INCLUDED_SPRITESHEETBUTTON_

#include "Engine/UI/ActivatableWidget.hpp"

class SpriteSheet;

/////////////////////////////////////////////////////////////////////////////
class SpriteSheetButton : public ActivatableWidget
{
public:
	SpriteSheetButton();
	SpriteSheetButton( const Vec2f& screenCoords );
	SpriteSheetButton( const Vec2f& screenCoords, const Vec2f& dimensions );
	SpriteSheetButton( const Vec2f& screenCoords, const Vec2f& dimensions, const Vec2f& minTexCoords, const Vec2f& maxTexCoords );
	SpriteSheetButton( const SpriteSheetButton& otherWidget );
	~SpriteSheetButton();

	SpriteSheet*	GetSpriteSheet() { return m_sheet; }
	Vec2f			GetMinTexCoords() const { return m_minTexCoords; }
	Vec2f			GetMaxTexCoords() const { return m_maxTexCoords; }
	Vec2i			GetSheetCoords() const { return m_sheetCoords; }
	std::string		GetSheetName() const { return m_sheetName; }

	void			SetSpriteSheet( SpriteSheet* sheet ) { m_sheet = sheet; }
	void			SetMinTexCoords( const Vec2f& minTexCoords ) { m_minTexCoords = minTexCoords; }
	void			SetMaxTexCoords( const Vec2f& maxTexCoords ) { m_maxTexCoords = maxTexCoords; }
	void			SetSheetCoords( const Vec2i& sheetCoords ) { m_sheetCoords = sheetCoords; }
	void			SetSheetName( const std::string& sheetName ) { m_sheetName = sheetName; }
	void			ClearSpriteSheet();

	virtual void	Activate();

protected:
	SpriteSheet*	m_sheet;
	Vec2f			m_minTexCoords;
	Vec2f			m_maxTexCoords;
	Vec2i			m_sheetCoords;
	std::string		m_sheetName;

protected:
	virtual void	RenderIdle();
	virtual void	RenderFocused();
	virtual void	RenderPressed();

};

#endif