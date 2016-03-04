//---------------------------------------------------------------------------
// SheetLoader.cpp
//---------------------------------------------------------------------------

#include "Engine/Core/FileSystem.hpp"
#include "Engine/Core/TinyXMLHelper.hpp"

#include "Editor/SpriteSheetDisplay.hpp"
#include "Editor/SheetLoader.hpp"


//---------------------------------------------------------------------------
SheetLoader::SheetLoader()
{

}


//---------------------------------------------------------------------------
SheetLoader::~SheetLoader()
{

}


//---------------------------------------------------------------------------
void SheetLoader::ImportSheetsFromMetadata( const char* metadataFilePath, SpriteSheetDisplay* display )
{
	FileSystem f;
	std::vector< std::string > filepaths;
	f.EnumerateFilesInDirectory( metadataFilePath, filepaths );
	for ( unsigned int index = 0; index < ( unsigned int ) filepaths.size(); ++ index )
	{
		CreateFromXML( filepaths[ index ].c_str(), display );
	}
}


//---------------------------------------------------------------------------
void SheetLoader::CreateFromXML( const char* metadataFilePath, SpriteSheetDisplay* display )
{
	TiXmlDocument doc;
	if ( !doc.LoadFile( metadataFilePath ) ) return;

	TiXmlElement* sheet = doc.RootElement();
	std::string metadataPathAsString( metadataFilePath );
	std::string fileName = GetAttributeAsString( sheet, "file" );
	Vec2i dimensions = GetAttributeAsVec2i( sheet, "tileDim" );

	size_t pos = metadataPathAsString.find_last_of( "\\/" );
	if ( std::string::npos == pos ) return;

	std::string filePath = metadataPathAsString.substr( 0, pos );
	filePath += "\\" + fileName;
	display->SetSpriteSheet( filePath.c_str(), dimensions );
}