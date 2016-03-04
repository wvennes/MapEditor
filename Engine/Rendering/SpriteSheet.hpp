//---------------------------------------------------------------------------
// SpriteSheet.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_SPRITESHEET_
#define _INCLUDED_SPRITESHEET_

#include <string>

#include "Engine/Math/Vec2.hpp"

class Texture;

typedef Vec2i TileCoords;
typedef Vec2f TexCoords;

class SpriteSheet
{
public:
	SpriteSheet();
	SpriteSheet( const std::string& fileName, const TileCoords& tileCoords );
	SpriteSheet( const SpriteSheet& other );
	~SpriteSheet();

	Vec2i		GetTileDimensions() const { return m_tileDimensions; }
	int			GetTileIndexForTileCoords( const TileCoords& coords ) const;
	TileCoords	GetTileCoordsForTileIndex( int index ) const;

	TexCoords	GetMinTexCoordsForTile( const TileCoords& tileCoords ) const;
	TexCoords	GetMaxTexCoordsForTile( const TileCoords& tileCoords ) const;

	Texture*	GetTexture() const { return m_texture; }

private:
	void		Initialize();

private:
	Texture*	m_texture;
	Vec2i		m_tileDimensions;
	Vec2f		m_textureDivisions;

};


#endif