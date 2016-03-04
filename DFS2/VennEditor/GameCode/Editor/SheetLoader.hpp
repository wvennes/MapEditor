//---------------------------------------------------------------------------
// SheetLoader.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_SHEETLOADER_
#define _INCLUDED_SHEETLOADER_

class SpriteSheetDisplay;

/////////////////////////////////////////////////////////////////////////////
class SheetLoader
{
public:
	SheetLoader();
	~SheetLoader();

	void	ImportSheetsFromMetadata( const char* metadataFilePath, SpriteSheetDisplay* display );

private:
	void	CreateFromXML( const char* metadataFilePath, SpriteSheetDisplay* display );

};

#endif