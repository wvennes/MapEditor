//---------------------------------------------------------------------------
// SpriteSheetDisplay.cpp
//---------------------------------------------------------------------------

#include "Engine/Rendering/Material.hpp"
#include "Engine/Rendering/ShaderProgram.hpp"
#include "Engine/Rendering/SpriteSheet.hpp"
#include "Engine/Rendering/VennRenderer.hpp"

#include "Editor/SpriteSheetButton.hpp"
#include "Editor/SpriteSheetDisplay.hpp"

extern VennRenderer* g_theRenderer;


//---------------------------------------------------------------------------
SpriteSheetDisplay::SpriteSheetDisplay()
	: ContainerWidget()
	, m_numPages( 0 )
	, m_currentPage( 0 )
	, m_numTilesPerPage( 0 )
	, m_spriteSheet( nullptr )
	, m_idleTint( Colors::WHITE )
	, m_focusedTint( Colors::WHITE )
	, m_pressedTint( Colors::WHITE )
	, m_gridDimensions( 0, 0 )
{

}


//---------------------------------------------------------------------------
SpriteSheetDisplay::SpriteSheetDisplay( const Vec2f& screenCoords, const Vec2f& dimensions, const char* filePath, const Vec2i& sheetDimensions, const Vec2i& gridDimensions )
	: ContainerWidget( screenCoords, dimensions )
	, m_numPages( 0 )
	, m_currentPage( 0 )
	, m_numTilesPerPage( 0 )
	, m_spriteSheet( new SpriteSheet( filePath, sheetDimensions ) )
	, m_idleTint( Colors::WHITE )
	, m_focusedTint( Colors::WHITE )
	, m_pressedTint( Colors::WHITE )
	, m_gridDimensions( gridDimensions )
{
	Initialize( filePath );
}


//---------------------------------------------------------------------------
SpriteSheetDisplay::~SpriteSheetDisplay()
{
	if ( m_spriteSheet ) delete m_spriteSheet;
}


//---------------------------------------------------------------------------
void SpriteSheetDisplay::Initialize( const char* filePath )
{
	// do nothing until there's a valid sprite sheet
	if ( !m_spriteSheet ) return;

	// get the sheet name to store in the cell for updating event info
	std::string path( filePath );
	std::string fileName;
	size_t pos = path.find_last_of( "\\/" ) + 1;
	if ( pos == std::string::npos ) return;
	fileName = path.substr( pos );

	// create a material from this sheet if it doesn't exist
	TryCreateMaterial( filePath );

	// how many tiles total in the sheet?
	unsigned int numTiles = m_spriteSheet->GetTileDimensions().x * m_spriteSheet->GetTileDimensions().y;

	// how many tiles to display per page?
	m_numTilesPerPage = m_gridDimensions.x * m_gridDimensions.y;

	// how many pages?
	m_numPages = numTiles / m_numTilesPerPage;
	if ( m_numPages * m_numTilesPerPage < numTiles ) ++ m_numPages; // round up in case tiles overflow

	// make our child widgets... positions will be relative to window
	float colWidth = m_dimensions.x / ( float ) m_gridDimensions.x;
	float rowHeight = m_dimensions.y / ( float ) m_gridDimensions.y;
	Vec2f currentScreenPosition = Vec2f( 0.f, m_gridDimensions.y * rowHeight );
	unsigned int tileCount = 0;

	for ( unsigned int page = 0; page < m_numPages; ++ page )
	{
		for ( int y = 0; y < m_gridDimensions.y; ++ y )
		{
			if ( tileCount >= numTiles ) break;
			for ( int x = 0; x < m_gridDimensions.x; ++ x )
			{
				if ( tileCount >= numTiles ) break;

				// get our tilecoords in terms of the sprite sheet
				Vec2i tileCoords = m_spriteSheet->GetTileCoordsForTileIndex( tileCount );

				// make the button and add to children
				SpriteSheetButton* button = new SpriteSheetButton( currentScreenPosition, Vec2f( colWidth, rowHeight ) );
				button->SetMaterials( filePath, filePath, filePath );
				button->SetTints( m_idleTint, m_focusedTint, m_pressedTint );
				button->SetMinTexCoords( m_spriteSheet->GetMinTexCoordsForTile( tileCoords ) );
				button->SetMaxTexCoords( m_spriteSheet->GetMaxTexCoordsForTile( tileCoords ) );
				button->SetSheetCoords( tileCoords );
				button->SetSheetName( fileName );
				button->SetSpriteSheet( new SpriteSheet( *m_spriteSheet ) );
				AddChildWidget( button );

				// increment the tile count and advance
				++ tileCount;
				currentScreenPosition.x += colWidth + 2.f;
			}

			// reset x and advance y
			currentScreenPosition.x = 0.f;
			currentScreenPosition.y -= rowHeight + 2.f;
		}

		// reset the position for the new page
		currentScreenPosition = Vec2f( 0.f, m_gridDimensions.y * rowHeight );
	}
}


//---------------------------------------------------------------------------
void SpriteSheetDisplay::TryCreateMaterial( const char* filePath )
{
	if ( Material::GetMaterial( filePath ) != nullptr ) return;

	Material* mat = Material::CreateAndGetMaterial( filePath, new ShaderProgram( *Material::GetDefaultUnlitMaterial()->GetShaderProgram() ) );
	mat->SetDiffuseTexture( m_spriteSheet->GetTexture() );
}


//---------------------------------------------------------------------------
void SpriteSheetDisplay::SetSpriteSheet( const char* filePath, const Vec2i& sheetDimensions )
{
	// if we already have an existing sprite sheet... for now just replace the old one
	// TODO: maybe add a new sheet instead of replacing the old one? that way we can add more tiles...
	if ( m_spriteSheet )
	{
		delete m_spriteSheet;
		m_spriteSheet = nullptr;
		DestroyVector( m_widgetChildren );
	}

	// reinitialize
	m_spriteSheet = new SpriteSheet( filePath, sheetDimensions );
	m_currentPage = 0;
	Initialize( filePath );
	UpdateActivePage();
}


//---------------------------------------------------------------------------
void SpriteSheetDisplay::AddSpriteSheet( const char* filePath, const Vec2i& sheetDimensions )
{
	// TODO: add support for adding tiles from multiple sheeets
}


//---------------------------------------------------------------------------
void SpriteSheetDisplay::SetTints( const Rgba& idle, const Rgba& focused, const Rgba& pressed )
{
	SetIdleTint( idle );
	SetFocusedTint( focused );
	SetPressedTint( pressed );
}


//---------------------------------------------------------------------------
void SpriteSheetDisplay::SetActivateEvent( const char* eventName )
{
	for ( WidgetChildren::iterator iter = m_widgetChildren.begin(); iter != m_widgetChildren.end(); ++ iter )
	{
		SpriteSheetButton* button = dynamic_cast< SpriteSheetButton* >( *iter );
		if ( !button ) continue;
		button->SetOnActivateEvent( eventName );
	}
}


//---------------------------------------------------------------------------
void SpriteSheetDisplay::UpdateActivePage()
{
	unsigned int pageForIndex = 0;
	for ( unsigned int index = 0; index < ( unsigned int ) m_widgetChildren.size(); ++ index )
	{
		pageForIndex = index / m_numTilesPerPage;
		if ( pageForIndex != m_currentPage )
		{
			m_widgetChildren[ index ]->ChangeState( WIDGET_HIDDEN );
		}
		else
		{
			m_widgetChildren[ index ]->ChangeState( WIDGET_IDLE );
		}
	}
}


//---------------------------------------------------------------------------
void SpriteSheetDisplay::NextPage()
{
	++ m_currentPage;
	if ( m_currentPage >= m_numPages ) m_currentPage = 0;
	UpdateActivePage();
}


//---------------------------------------------------------------------------
void SpriteSheetDisplay::PreviousPage()
{
	if ( m_numPages == 0 )
	{
		m_currentPage = 0;
		return;
	}
	if ( m_currentPage == 0 ) m_currentPage = m_numPages - 1;
	else -- m_currentPage;
	UpdateActivePage();
}


//---------------------------------------------------------------------------
void SpriteSheetDisplay::Render()
{
	if ( m_widgetChildren.empty() || m_widgetState == WIDGET_HIDDEN ) return;

	g_theRenderer->PushMatrix();
	g_theRenderer->ApplyTransformation( m_parentToLocal );
	for ( unsigned int index = 0; index < m_numTilesPerPage; ++ index )
	{
		unsigned int actualIndex = index + m_currentPage * m_numTilesPerPage;
		if ( actualIndex >= m_widgetChildren.size() ) break;

		m_widgetChildren[ actualIndex ]->Render();
	}
	g_theRenderer->PopMatrix();
}