//---------------------------------------------------------------------------
// TheEditor.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_THEEDITOR_
#define _INCLUDED_THEEDITOR_

class Widget;
class SpriteSheetDisplay;
class SpriteSheetButton;
class TileGridWidget;
class Event;

/////////////////////////////////////////////////////////////////////////////
class TheEditor
{
public:
	TheEditor();
	~TheEditor();

	bool					ProcessInput();
	// input events
//	void		OnKeyDown( Event& e );

	bool					Update();
	void					Render();

private:
	TileGridWidget*			m_mapWidget;
	SpriteSheetDisplay*		m_sheetDisplayWidget;
	SpriteSheetButton*		m_currentlySelectedTile;


private:
	void					OnNextPage( Event& e );
	void					OnPrevPage( Event& e );
	void					OnReimport( Event& e );
	void					OnSelectTile( Event& e );
	void					OnPlaceTile( Event& e );
	void					OnExport( Event& e );
	void					OnImport( Event& e );

	void					RegisterEvents();
	void					UnregisterEvents();
	void					InitializeWidgets();
	void					InitializeTestWidgets();
	void					InitializeMaterials();

	void					PlaceTileGridWidget();
	void					PlaceSpritesheetGridWidget();
	void					PlaceButtons();
	void					PlaceCurrentlySelectedTileWindow();

};

#endif