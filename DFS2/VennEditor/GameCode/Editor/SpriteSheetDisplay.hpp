//---------------------------------------------------------------------------
// SpriteSheetDisplay.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_SPRITESHEETDISPLAY_
#define _INCLUDED_SPRITESHEETDISPLAY_

#include "Engine/UI/ContainerWidget.hpp"

class SpriteSheet;
typedef std::vector< SpriteSheet* >	SpriteSheets;

/////////////////////////////////////////////////////////////////////////////
class SpriteSheetDisplay : public ContainerWidget
{
public:
	SpriteSheetDisplay();
	SpriteSheetDisplay( const Vec2f& screenCoords, const Vec2f& dimensions, const char* filePath, const Vec2i& sheetDimensions, const Vec2i& gridDimensions );
	~SpriteSheetDisplay();

	const SpriteSheet*	GetSpriteSheet() const { return m_spriteSheet; }
	Vec2i				GetGridDimensions() const { return m_gridDimensions; }
	Rgba				GetIdleTint() const { return m_idleTint; }
	Rgba				GetFocusedTint() const { return m_focusedTint; }
	Rgba				GetPressedTint() const { return m_pressedTint; }

	void				SetSpriteSheet( const char* filePath, const Vec2i& sheetDimensions );
	void				AddSpriteSheet( const char* filePath, const Vec2i& sheetDimensions );
	void				SetWidgetTileDimensions( const Vec2i& dimensions ) { m_gridDimensions = dimensions; }
	void				SetTints( const Rgba& idle, const Rgba& focused, const Rgba& pressed );
	void				SetIdleTint( const Rgba& color ) { m_idleTint = color; }
	void				SetFocusedTint( const Rgba& color ) { m_focusedTint = color; }
	void				SetPressedTint( const Rgba& color ) { m_pressedTint = color; }
	void				SetActivateEvent( const char* eventName );

	void				NextPage();
	void				PreviousPage();

	virtual void		Render();

protected:
	unsigned int		m_numPages;
	unsigned int		m_currentPage;
	unsigned int		m_numTilesPerPage;
	SpriteSheet*		m_spriteSheet;
	Rgba				m_idleTint;
	Rgba				m_focusedTint;
	Rgba				m_pressedTint;
	Vec2i				m_gridDimensions;
	//SpriteSheets		m_spriteSheets;
	
private:
	void				TryCreateMaterial( const char* filePath );
	void				UpdateActivePage();
	void				Initialize( const char* filePath );

};

#endif