//---------------------------------------------------------------------------
// SpriteSheet.cpp
//---------------------------------------------------------------------------

#include "Engine/Rendering/Texture.hpp"
#include "Engine/Rendering/SpriteSheet.hpp"


//---------------------------------------------------------------------------
SpriteSheet::SpriteSheet()
	: m_texture( nullptr )
	, m_tileDimensions( 0, 0 )
	, m_textureDivisions( 0.f, 0.f )
{

}


//---------------------------------------------------------------------------
SpriteSheet::SpriteSheet( const std::string& fileName, const TileCoords& tileCoords )
	: m_texture( nullptr )
	, m_tileDimensions( tileCoords )
	, m_textureDivisions( 1.f / ( float ) tileCoords.x, 1.f / ( float ) tileCoords.y )
{
	m_texture = Texture::CreateOrGetTexture( fileName );
}


//---------------------------------------------------------------------------
SpriteSheet::SpriteSheet( const SpriteSheet& other )
	: m_texture( other.m_texture )
	, m_tileDimensions( other.m_tileDimensions )
	, m_textureDivisions( other.m_textureDivisions )
{

}


//---------------------------------------------------------------------------
SpriteSheet::~SpriteSheet()
{

}


//---------------------------------------------------------------------------
TexCoords SpriteSheet::GetMinTexCoordsForTile( const TileCoords& tileCoords ) const
{
	return TexCoords( tileCoords.x * m_textureDivisions.x, tileCoords.y * m_textureDivisions.y );
}


//---------------------------------------------------------------------------
int SpriteSheet::GetTileIndexForTileCoords( const TileCoords& coords ) const
{
	return coords.y * m_tileDimensions.x + coords.x;
}


//---------------------------------------------------------------------------
TileCoords SpriteSheet::GetTileCoordsForTileIndex( int index ) const
{
	return TileCoords( index % m_tileDimensions.x, index / m_tileDimensions.x );
}


//---------------------------------------------------------------------------
TexCoords SpriteSheet::GetMaxTexCoordsForTile( const TileCoords& tileCoords ) const
{
	return GetMinTexCoordsForTile( tileCoords ) + m_textureDivisions;
}