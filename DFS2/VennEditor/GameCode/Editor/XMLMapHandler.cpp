//---------------------------------------------------------------------------
// XMLMapHandler.cpp
//---------------------------------------------------------------------------

#include "Engine/Core/TinyXMLHelper.hpp"
#include "Engine/Rendering/Material.hpp"
//#include "Engine/Rendering/SpriteSheet.hpp"
#include "Engine/Rendering/ShaderProgram.hpp"

#include "Editor/TileGridWidget.hpp"
#include "Editor/SpriteSheetGridButton.hpp"
#include "Editor/XMLMapHandler.hpp"


//---------------------------------------------------------------------------
XMLMapHandler::XMLMapHandler()
{

}


//---------------------------------------------------------------------------
XMLMapHandler::~XMLMapHandler()
{

}


//---------------------------------------------------------------------------
void XMLMapHandler::SaveFileFromWidget( const char* filePath, TileGridWidget* widget )
{
	// tinyxml setup
	TiXmlDocument doc;
	TiXmlDeclaration* declaration = new TiXmlDeclaration( "1.0", "utf-8", "" );
	doc.LinkEndChild( declaration );

	// <MapData></MapData>
	TiXmlElement* root = new TiXmlElement( "Map" );
	doc.LinkEndChild( root );

	// sheet legend
	ExportSheetLegend sheetLegend;
	TiXmlElement* sheetLegendNode = new TiXmlElement( "SheetLegend" );
	root->LinkEndChild( sheetLegendNode );

	// tile legend
	ExportTileLegend tileLegend;
	TiXmlElement* tileLegendNode = new TiXmlElement( "TileLegend" );
	root->LinkEndChild( tileLegendNode );

	// map data
	TiXmlElement* mapDataNode = new TiXmlElement( "MapData" );
	mapDataNode->SetAttribute( "sizeX", widget->GetGridDimensions().x ); // map size x
	mapDataNode->SetAttribute( "sizeY", widget->GetGridDimensions().y ); // map size y
	root->LinkEndChild( mapDataNode );

	// row node
	TiXmlElement* rowDataNode = new TiXmlElement( "RowData" );
	mapDataNode->LinkEndChild( rowDataNode );

	// create legends and export map data
	int sheetId = 0;
	int tileId = 0;
	int currentSheetId = 0;
	int lastTileId = 0;
	int currentTileCount = 0; // we're going to save by count to save some space
	int currentRowCount = 0;
	Vec2i currentSheetCoords;
	TileInfo info;

	// insert an empty tile entry
	TileInfo blankInfo;
	blankInfo.m_tileId = -1;
	blankInfo.m_sheetId = -1;
	tileLegend.insert( std::make_pair( blankInfo, -1 ) );

	const WidgetChildren* gridTiles = widget->GetWidgetChildren();
	for ( WidgetChildren::const_iterator iter = gridTiles->begin(); iter != gridTiles->end(); ++ iter )
	{
		SpriteSheetGridButton* button = dynamic_cast< SpriteSheetGridButton* >( *iter );
		if ( !button ) continue;

		// insert a new sheet entry if this is new
		if ( button->GetSheetName() != "null" && sheetLegend.find( button->GetSheetName() ) == sheetLegend.end() )
		{
			sheetLegend.insert( std::make_pair( button->GetSheetName(), sheetId ) );
			TiXmlElement* entry = new TiXmlElement( "Entry" );
			entry->SetAttribute( "id", sheetId );
			entry->SetAttribute( "name", button->GetSheetName().c_str() );

			if ( button->GetSpriteSheet() != nullptr )
			{
				SpriteSheet* sheet = button->GetSpriteSheet();
				entry->SetAttribute( "sheetDimensionsX", sheet->GetTileDimensions().x );
				entry->SetAttribute( "sheetDimensionsY", sheet->GetTileDimensions().y );
			}
			else
			{
				entry->SetAttribute( "sheetDimensionsX", 0 );
				entry->SetAttribute( "sheetDimensionsY", 0 );
			}

			sheetLegendNode->LinkEndChild( entry );

			++ sheetId;
		}

		if ( button->GetSheetName() == "null" ) currentSheetId = -1;
		else currentSheetId = sheetLegend[ button->GetSheetName() ];
		currentSheetCoords = button->GetSheetCoords();
		info.m_sheetId = currentSheetId;
		info.m_sheetCoords = currentSheetCoords;
		info.m_tileId = -1;

		// insert a new tile entry if this is new
		if ( currentSheetId != -1 && tileLegend.find( info ) == tileLegend.end() )
		{
			info.m_tileId = tileId;
			tileLegend.insert( std::make_pair( info, tileId ) );
			TiXmlElement* entry = new TiXmlElement( "Entry" );
			entry->SetAttribute( "id", tileId );
			entry->SetAttribute( "sheetId", currentSheetId );
			entry->SetAttribute( "sheetX", currentSheetCoords.x );
			entry->SetAttribute( "sheetY", currentSheetCoords.y );
			tileLegendNode->LinkEndChild( entry );

			++ tileId;
		}

		// new row check
		if ( currentRowCount >= widget->GetGridDimensions().x )
		{
			// add the current count of tiles to this row
			if ( currentTileCount > 0 )
			{
				TiXmlElement* tiles = new TiXmlElement( "Tiles" );
				tiles->SetAttribute( "tileId", lastTileId );
				tiles->SetAttribute( "count", currentTileCount );
				rowDataNode->LinkEndChild( tiles );
				currentTileCount = 0;
				lastTileId = tileLegend[ info ];
			}
			rowDataNode = new TiXmlElement( "RowData" );
			mapDataNode->LinkEndChild( rowDataNode );
			currentRowCount = 0;
		}

		// add current count of tiles to this row
		if ( lastTileId != tileLegend[ info ] && currentTileCount > 0 )
		{
			TiXmlElement* tiles = new TiXmlElement( "Tiles" );
			tiles->SetAttribute( "tileId", lastTileId );
			tiles->SetAttribute( "count", currentTileCount );
			rowDataNode->LinkEndChild( tiles );
			currentTileCount = 0;
			lastTileId = tileLegend[ info ];
		}

		++ currentTileCount;
		++ currentRowCount;
	}

	// export one more node to grab the remaining tiles
	TiXmlElement* tiles = new TiXmlElement( "Tiles" );
	tiles->SetAttribute( "tileId", lastTileId );
	tiles->SetAttribute( "count", currentTileCount );
	rowDataNode->LinkEndChild( tiles );

	// save xml file
	doc.SaveFile( filePath );
}


//---------------------------------------------------------------------------
void XMLMapHandler::LoadFileToWidget( const char* filePath, TileGridWidget* widget )
{
	TiXmlDocument doc;
	if ( !doc.LoadFile( filePath ) ) return;

	TiXmlElement* root = doc.RootElement();
	TiXmlElement* sheetLegendNode = root->FirstChildElement( "SheetLegend" );
	TiXmlElement* tileLegendNode = root->FirstChildElement( "TileLegend" );
	TiXmlElement* mapDataNode = root->FirstChildElement( "MapData" );
	Vec2i gridDimensions( GetAttributeAsInt( mapDataNode, "sizeX" ), GetAttributeAsInt( mapDataNode, "sizeY" ) );

	ImportSheetLegend sheetLegend;
	ImportTileLegend tileLegend;
	std::string sheetFolder( "./Data/Sheets/" );

	// clear out the widget
	widget->ResetGrid( gridDimensions );

	// read in the sheet legend
	for ( TiXmlElement* node = sheetLegendNode->FirstChildElement( "Entry" ); node != nullptr; node = node->NextSiblingElement( "Entry" ) )
	{
		std::string sheetName;
		int sheetId = 0;
		Vec2i sheetDimensions;

		sheetId = GetAttributeAsInt( node, "id" );
		sheetName = GetAttributeAsString( node, "name" );
		sheetDimensions.x = GetAttributeAsInt( node, "sheetDimensionsX" );
		sheetDimensions.y = GetAttributeAsInt( node, "sheetDimensionsY" );

		sheetLegend.insert( std::make_pair( sheetId, SheetInfo() ) );
		SheetInfo& info = sheetLegend[ sheetId ];
		info.m_sheetId = sheetId;
		info.m_sheetName = sheetName;
		info.m_sheet = new SpriteSheet( sheetFolder + sheetName, sheetDimensions );
		TryCreateMaterial( sheetFolder + sheetName, info.m_sheet );
	}

	// insert an entry for empty sheet
	sheetLegend.insert( std::make_pair( -1, SheetInfo() ) );

	// read in the tile legend
	for ( TiXmlElement* node = tileLegendNode->FirstChildElement( "Entry" ); node != nullptr; node = node->NextSiblingElement( "Entry" ) )
	{
		int tileId = 0;
		int sheetId = 0;
		Vec2i sheetCoords;

		tileId = GetAttributeAsInt( node, "id" );
		sheetId = GetAttributeAsInt( node, "sheetId" );
		sheetCoords.x = GetAttributeAsInt( node, "sheetX" );
		sheetCoords.y = GetAttributeAsInt( node, "sheetY" );

		TileInfo info;
		info.m_tileId = tileId;
		info.m_sheetId = sheetId;
		info.m_sheetCoords = sheetCoords;

		tileLegend.insert( std::make_pair( tileId, info ) );
	}

	// insert an entry for empty tile
	TileInfo emptyTile;
	emptyTile.m_sheetId = -1;
	emptyTile.m_tileId = -1;
	tileLegend.insert( std::make_pair( -1, emptyTile ) );

	// read in the map data
	unsigned int currentIndex = 0;
	for ( TiXmlElement* row = mapDataNode->FirstChildElement( "RowData" ); row != nullptr; row = row->NextSiblingElement( "RowData" ) )
	{
		for ( TiXmlElement* node = row->FirstChildElement( "Tiles" ); node != nullptr; node = node->NextSiblingElement( "Tiles" ) )
		{
			int tileId = GetAttributeAsInt( node, "tileId" );
			int count = GetAttributeAsInt( node, "count" );

			// grab the legend entries
			TileInfo& tileInfo = tileLegend[ tileId ];
			SheetInfo& sheetInfo = sheetLegend[ tileInfo.m_sheetId ];
			std::string matName( "defaultUnlit" );

			if ( sheetInfo.m_sheetName != "null" )
			{
				matName = sheetFolder + sheetInfo.m_sheetName;
			}

			// calculate tex coords
			Vec2f minTexCoords;
			Vec2f maxTexCoords;
			if ( sheetInfo.m_sheet )
			{
				minTexCoords = sheetInfo.m_sheet->GetMinTexCoordsForTile( tileInfo.m_sheetCoords );
				maxTexCoords = sheetInfo.m_sheet->GetMaxTexCoordsForTile( tileInfo.m_sheetCoords );
			}

			// loop through the current count and place the correct data
			for ( int x = 0; x < count; ++ x )
			{
				const WidgetChildren* children = widget->GetWidgetChildren();
				SpriteSheetGridButton* button = dynamic_cast< SpriteSheetGridButton* >( ( *children )[ currentIndex ] );

				if ( !button ) return; // error, wrong widget being passed in or widget data is invalid

				button->SetSheetCoords( tileInfo.m_sheetCoords );
				button->SetSheetName( sheetInfo.m_sheetName );
				if ( sheetInfo.m_sheet ) button->SetSpriteSheet( new SpriteSheet( *sheetInfo.m_sheet ) );
				button->SetMinTexCoords( minTexCoords );
				button->SetMaxTexCoords( maxTexCoords );
				button->SetMaterials( matName, matName, matName );

				++ currentIndex;
			}
		}
	}

	widget->SetActivateEvent( "placeTile" );
}


//---------------------------------------------------------------------------
void XMLMapHandler::TryCreateMaterial( const std::string& filePath, SpriteSheet* sheet )
{
	if ( Material::GetMaterial( filePath ) != nullptr ) return;

	Material* mat = Material::CreateAndGetMaterial( filePath, new ShaderProgram( *Material::GetDefaultUnlitMaterial()->GetShaderProgram() ) );
	mat->SetDiffuseTexture( sheet->GetTexture() );
}