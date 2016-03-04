//---------------------------------------------------------------------------
// SpriteSheetGridButton.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_SPRITESHEETGRIDBUTTON_
#define _INCLUDED_SPRITESHEETGRIDBUTTON_

#include "Editor/SpriteSheetButton.hpp"

/////////////////////////////////////////////////////////////////////////////
class SpriteSheetGridButton : public SpriteSheetButton
{
public:
	SpriteSheetGridButton();
	SpriteSheetGridButton( const Vec2f& screenCoords );
	SpriteSheetGridButton( const Vec2f& screenCoords, const Vec2f& dimensions );
	SpriteSheetGridButton( const Vec2f& screenCoords, const Vec2f& dimensions, const Vec2f& minTexCoords, const Vec2f& maxTexCoords );
	SpriteSheetGridButton( const SpriteSheetGridButton& otherWidget );
	~SpriteSheetGridButton();

	Vec2i			GetGridCoords() const { return m_gridCoords; }
	void			SetGridCoords( const Vec2i& gridCoords ) { m_gridCoords = gridCoords; }

	virtual void	Activate();

private:
	Vec2i			m_gridCoords;

};

#endif