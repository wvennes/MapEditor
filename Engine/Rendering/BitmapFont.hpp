//---------------------------------------------------------------------------
// BitmapFont.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_BITMAPFONT_
#define _INCLUDED_BITMAPFONT_

#include <string>
#include <map>
#include <vector>
#include "Engine/Math/Vec2.hpp"

class Texture;
class ZipFile;
class TiXmlDocument;
class BitmapFont;
struct GlyphMetaData;

typedef std::map< std::string, BitmapFont* >	BitmapFontMap;

/////////////////////////////////////////////////////////////////////////////
class BitmapFont
{
public:
	BitmapFont( const std::string& metaDataFilePath );
	BitmapFont( ZipFile* zipFile, const char* metaDataFilePath );
	BitmapFont( const BitmapFont& other );
	~BitmapFont();

	static BitmapFont*				CreateAndGetFont( const std::string& metaDataFilePath );
	static BitmapFont*				CreateAndGetFont( ZipFile* zipFile, const char* metaDataFilePath );
	static BitmapFont*				CreateOrGetFont( const char* metaDataFilePath );
	static void						FreeBitmapFontRegistryMemory();

public:
	std::vector< Texture* >			m_glyphSheets;
	std::map< int, GlyphMetaData >	m_glyphData;

private:
	BitmapFont();
	bool							LoadMetaDataWithTinyXML( const std::string& metaDataFilePath );
	bool							LoadMetaDataWithTinyXML( ZipFile* zipFile, const char* metaDataFilePath );
	void							ExtractMetaDataFromTinyXMLDocument( TiXmlDocument& metaDataDocument );
	bool							AddTexturesFromFolder( TiXmlDocument& metaDataDocument, const std::string& metaDataFilePath );
	bool							AddTexturesFromZip( TiXmlDocument& metaDataDocument, ZipFile* zipFile, const std::string& metaDataFilePath );

private:
	static BitmapFontMap			s_bitmapFontRegistry;

};


/////////////////////////////////////////////////////////////////////////////
struct GlyphMetaData
{
	GlyphMetaData() : m_minTexCoords( 0.f, 0.f ), m_maxTexCoords( 0.f, 0.f ), m_ttfA( 0.f ), m_ttfB( 0.f ), m_ttfC( 0.f ), m_glyphSheetIndex( 0 ) {}
	GlyphMetaData( const GlyphMetaData& other )
		: m_minTexCoords( other.m_minTexCoords )
		, m_maxTexCoords( other.m_maxTexCoords )
		, m_ttfA( other.m_ttfA )
		, m_ttfB( other.m_ttfB )
		, m_ttfC( other.m_ttfC )
		, m_glyphSheetIndex( other.m_glyphSheetIndex )
	{

	}

	Vec2f	m_minTexCoords;
	Vec2f	m_maxTexCoords;
	float	m_ttfA;
	float	m_ttfB;
	float	m_ttfC;
	char	m_glyphSheetIndex;
};

extern bool g_isQuitting;

#endif