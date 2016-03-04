//---------------------------------------------------------------------------
// TileGridWidget.cpp
//---------------------------------------------------------------------------

#include "Engine/Core/EngineCommon.hpp"

#include "Editor/SpriteSheetGridButton.hpp"
#include "Editor/TileGridWidget.hpp"


//---------------------------------------------------------------------------
TileGridWidget::TileGridWidget( const Vec2f& screenCoords, const Vec2f& widgetDimensions, const Vec2i& gridDimensions )
	: ContainerWidget( screenCoords, widgetDimensions )
	, m_gridDimensions( gridDimensions )
	, m_idleTint( Colors::WHITE )
	, m_focusedTint( Colors::WHITE )
	, m_pressedTint( Colors::WHITE )
{
	PlaceTileWidgets();
}


//---------------------------------------------------------------------------
TileGridWidget::TileGridWidget( const Vec2f& screenCoords, const Vec2f& widgetDimensions, const Vec2i& gridDimensions, const Rgba& idleTint, const Rgba& focusedTint, const Rgba& pressedTint )
	: ContainerWidget( screenCoords, widgetDimensions )
	, m_gridDimensions( gridDimensions )
	, m_idleTint( idleTint )
	, m_focusedTint( focusedTint )
	, m_pressedTint( pressedTint )
{
	PlaceTileWidgets();
}


//---------------------------------------------------------------------------
TileGridWidget::~TileGridWidget()
{

}


//---------------------------------------------------------------------------
void TileGridWidget::ResetGrid( const Vec2i& gridDimensions )
{
	m_gridDimensions = gridDimensions;
	DestroyVector( m_widgetChildren );
	PlaceTileWidgets();
}


//---------------------------------------------------------------------------
void TileGridWidget::SetActivateEvent( const char* eventName )
{
	for ( WidgetChildren::iterator iter = m_widgetChildren.begin(); iter != m_widgetChildren.end(); ++ iter )
	{
		SpriteSheetGridButton* button = dynamic_cast< SpriteSheetGridButton* >( *iter );
		if ( !button ) continue;
		button->SetOnActivateEvent( eventName );
	}
}


//---------------------------------------------------------------------------
void TileGridWidget::PlaceTileWidgets()
{
	float colWidth = m_dimensions.x / ( float ) m_gridDimensions.x;
	float rowHeight = m_dimensions.y / ( float ) m_gridDimensions.y;

	Vec2f currentScreenPosition;
	
	for ( int y = 0; y < m_gridDimensions.y; ++ y )
	{
		//PlaceTileWidget( currentScreenPosition, Vec2f( colWidth, rowHeight ) );
		for ( int x = 0; x < m_gridDimensions.x; ++ x )
		{
			PlaceTileWidget( currentScreenPosition, Vec2f( colWidth, rowHeight ), Vec2i( x, y ) );
			currentScreenPosition.x += colWidth;
		}

		currentScreenPosition.x = 0.f;
		currentScreenPosition.y += rowHeight;
	}
}


//---------------------------------------------------------------------------
void TileGridWidget::PlaceTileWidget( const Vec2f& position, const Vec2f& dimensions, const Vec2i& gridCoords )
{
	SpriteSheetGridButton* widget = new SpriteSheetGridButton();
	widget->SetDimensions( dimensions );
	widget->SetScreenPosition( position );
	widget->SetIdleMaterial( "defaultUnlit" );
	widget->SetFocusedMaterial( "defaultUnlit" );
	widget->SetPressedMaterial( "defaultUnlit" );
	widget->SetIdleTint( m_idleTint );
	widget->SetFocusedTint( m_focusedTint );
	widget->SetPressedTint( m_pressedTint );
	widget->SetGridCoords( gridCoords );
	AddChildWidget( widget );
}


//---------------------------------------------------------------------------
void TileGridWidget::Render()
{
	EnableStencilTest();
	Widget::Render();
	DisableStencilTest();
}