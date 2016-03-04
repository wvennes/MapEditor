//---------------------------------------------------------------------------
// TileGridWidget.hpp
//---------------------------------------------------------------------------


#pragma once
#ifndef _INCLUDED_TILEGRIDWIDGET_
#define _INCLUDED_TILEGRIDWIDGET_

#include <vector>

#include "Engine/Math/Colors.hpp"
#include "Engine/UI/ActivatableWidget.hpp"
#include "Engine/UI/ContainerWidget.hpp"

/////////////////////////////////////////////////////////////////////////////
class TileGridWidget : public ContainerWidget
{
public:
	TileGridWidget( const Vec2f& screenCoords, const Vec2f& widgetDimensions, const Vec2i& gridDimensions );
	TileGridWidget( const Vec2f& screenCoords, const Vec2f& widgetDimensions, const Vec2i& gridDimensions, const Rgba& idleTint, const Rgba& focusedTint, const Rgba& pressedTint );
	~TileGridWidget();

	void					SetActivateEvent( const char* eventName );
	void					SetIdleTint( const Rgba& color ) { m_idleTint = color; }
	void					SetFocusedTint( const Rgba& color ) { m_focusedTint = color; }
	void					SetPressedTint( const Rgba& color ) { m_pressedTint = color; }
	Rgba					GetIdleTint() const { return m_idleTint; }
	Rgba					GetFocusedTint() const { return m_focusedTint; }
	Rgba					GetPressedTint() const { return m_pressedTint; }
	Vec2i					GetGridDimensions() const { return m_gridDimensions; }
	void					ResetGrid( const Vec2i& gridDimensions );

	inline Vec2i			GetTileCoordsForIndex( unsigned int index ) const;
	inline unsigned int		GetIndexForTileCoords( const Vec2i& tileCoords ) const;

	const WidgetChildren*	GetWidgetChildren() const { return &m_widgetChildren; }

	void					Render();

protected:
	Vec2i					m_gridDimensions;
	Rgba					m_idleTint;
	Rgba					m_focusedTint;
	Rgba					m_pressedTint;

protected:
	void					PlaceTileWidgets();
	void					PlaceTileWidget( const Vec2f& position, const Vec2f& dimensions, const Vec2i& gridCoords );

};


//---------------------------------------------------------------------------
Vec2i TileGridWidget::GetTileCoordsForIndex( unsigned int index ) const
{
	return Vec2i( index % m_gridDimensions.x, index / m_gridDimensions.x );
}


//---------------------------------------------------------------------------
unsigned int TileGridWidget::GetIndexForTileCoords( const Vec2i& tileCoords ) const
{
	return tileCoords.y * m_gridDimensions.x + tileCoords.x;
}

#endif