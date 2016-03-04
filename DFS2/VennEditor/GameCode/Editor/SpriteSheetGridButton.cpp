//---------------------------------------------------------------------------
// SpriteSheetGridButton.cpp
//---------------------------------------------------------------------------

#include "Engine/Core/EventSystem.hpp"
#include "Engine/Rendering/SpriteSheet.hpp"

#include "Editor/SpriteSheetGridButton.hpp"


//---------------------------------------------------------------------------
SpriteSheetGridButton::SpriteSheetGridButton()
	: SpriteSheetButton()
	, m_gridCoords( 0, 0 )
{

}


//---------------------------------------------------------------------------
SpriteSheetGridButton::SpriteSheetGridButton( const Vec2f& screenCoords )
	: SpriteSheetButton( screenCoords )
	, m_gridCoords( 0, 0 )
{

}


//---------------------------------------------------------------------------
SpriteSheetGridButton::SpriteSheetGridButton( const Vec2f& screenCoords, const Vec2f& dimensions )
	: SpriteSheetButton( screenCoords, dimensions )
	, m_gridCoords( 0, 0 )
{

}


//---------------------------------------------------------------------------
SpriteSheetGridButton::SpriteSheetGridButton( const Vec2f& screenCoords, const Vec2f& dimensions, const Vec2f& minTexCoords, const Vec2f& maxTexCoords )
	: SpriteSheetButton( screenCoords, dimensions, minTexCoords, maxTexCoords )
	, m_gridCoords( 0, 0 )
{

}


//---------------------------------------------------------------------------
SpriteSheetGridButton::SpriteSheetGridButton( const SpriteSheetGridButton& otherWidget )
	: SpriteSheetButton( otherWidget )
	, m_gridCoords( otherWidget.m_gridCoords )
{

}


//---------------------------------------------------------------------------
SpriteSheetGridButton::~SpriteSheetGridButton()
{

}


//---------------------------------------------------------------------------
void SpriteSheetGridButton::Activate()
{
	std::string eventName;
	if ( !m_widgetEvents.GetProperty( "onActivate", eventName ) ) return;

	Event* e = EventSystem::GetInstance()->GetEvent( eventName );
	if ( !e ) return;

	e->AddOrChangeProperty( "minTexCoords", m_minTexCoords );
	e->AddOrChangeProperty( "maxTexCoords", m_maxTexCoords );
	e->AddOrChangeProperty( "material", m_idleMaterial );
	e->AddOrChangeProperty( "context", this );
	e->AddOrChangeProperty( "sheetCoords", m_sheetCoords );
	e->AddOrChangeProperty( "sheetName", m_sheetName );
	e->AddOrChangeProperty( "sheet", m_sheet );
	e->AddOrChangeProperty( "gridCoords", m_gridCoords );

	EventSystem::GetInstance()->FireEvent( eventName );
}