//---------------------------------------------------------------------------
// XMLMapHandler.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_XMLMAPHANDLER_
#define _INCLUDED_XMLMAPHANDLER_

#include <map>
#include <string>
#include "Engine/Math/Vec2.hpp"
#include "Engine/Rendering/SpriteSheet.hpp"

class TileGridWidget;
struct TileInfo;
struct SheetInfo;

typedef std::map< std::string, int >	ExportSheetLegend;
typedef std::map< TileInfo, int >		ExportTileLegend;
typedef std::map< int, SheetInfo >		ImportSheetLegend;
typedef std::map< int, TileInfo >		ImportTileLegend;

/////////////////////////////////////////////////////////////////////////////
struct TileInfo
{
	int				m_tileId;
	int				m_sheetId;
	Vec2i			m_sheetCoords;

	bool operator<( const TileInfo& rhs ) const
	{
		if ( m_sheetId != rhs.m_sheetId )
		{
			return m_sheetId < rhs.m_sheetId;
		}
		else return m_sheetCoords < rhs.m_sheetCoords;
	}
};


/////////////////////////////////////////////////////////////////////////////
struct SheetInfo
{
	SheetInfo() : m_sheetId( 0 ), m_sheet( nullptr ), m_sheetName( "null" ) {}
	~SheetInfo()
	{
		if ( m_sheet ) delete m_sheet;
	}

	int				m_sheetId;
	SpriteSheet*	m_sheet;
	std::string		m_sheetName;
};


/////////////////////////////////////////////////////////////////////////////
class XMLMapHandler
{
public:
	XMLMapHandler();
	~XMLMapHandler();

	void	SaveFileFromWidget( const char* filePath, TileGridWidget* widget );
	void	LoadFileToWidget( const char* filePath, TileGridWidget* widget );

private:
	void	TryCreateMaterial( const std::string& sheetName, SpriteSheet* sheet );
};


#endif