//---------------------------------------------------------------------------
// SpriteSheetButton.cpp
//---------------------------------------------------------------------------

#include "Engine/Rendering/Material.hpp"
#include "Engine/Rendering/RenderFunctions.hpp"
#include "Engine/Core/EventSystem.hpp"
#include "Engine/Rendering/SpriteSheet.hpp"

#include "Editor/SpriteSheetButton.hpp"


//---------------------------------------------------------------------------
SpriteSheetButton::SpriteSheetButton()
	: ActivatableWidget()
	, m_sheet( nullptr )
	, m_minTexCoords( 0.f, 0.f )
	, m_maxTexCoords( 0.f, 0.f )
	, m_sheetCoords( 0, 0 )
	, m_sheetName( "null" )
{

}


//---------------------------------------------------------------------------
SpriteSheetButton::SpriteSheetButton( const Vec2f& screenCoords )
	: ActivatableWidget( screenCoords )
	, m_sheet( nullptr )
	, m_minTexCoords( 0.f, 0.f )
	, m_maxTexCoords( 0.f, 0.f )
	, m_sheetCoords( 0, 0 )
	, m_sheetName( "null" )
{

}


//---------------------------------------------------------------------------
SpriteSheetButton::SpriteSheetButton( const Vec2f& screenCoords, const Vec2f& dimensions )
	: ActivatableWidget( screenCoords, dimensions )
	, m_sheet( nullptr )
	, m_minTexCoords( 0.f, 0.f )
	, m_maxTexCoords( 0.f, 0.f )
	, m_sheetCoords( 0, 0 )
	, m_sheetName( "null" )
{

}


//---------------------------------------------------------------------------
SpriteSheetButton::SpriteSheetButton( const Vec2f& screenCoords, const Vec2f& dimensions, const Vec2f& minTexCoords, const Vec2f& maxTexCoords )
	: ActivatableWidget( screenCoords, dimensions )
	, m_sheet( nullptr )
	, m_minTexCoords( minTexCoords )
	, m_maxTexCoords( maxTexCoords )
	, m_sheetCoords( 0, 0 )
	, m_sheetName( "null" )
{

}


//---------------------------------------------------------------------------
SpriteSheetButton::SpriteSheetButton( const SpriteSheetButton& otherWidget )
	: ActivatableWidget( otherWidget )
	, m_sheet( nullptr )
	, m_minTexCoords( otherWidget.m_minTexCoords )
	, m_maxTexCoords( otherWidget.m_maxTexCoords )
	, m_sheetCoords( otherWidget.m_sheetCoords )
	, m_sheetName( otherWidget.m_sheetName )
{

}


//---------------------------------------------------------------------------
SpriteSheetButton::~SpriteSheetButton()
{
	if ( m_sheet ) delete m_sheet;
}


//---------------------------------------------------------------------------
void SpriteSheetButton::ClearSpriteSheet()
{
	if ( m_sheet ) delete m_sheet;
	m_sheet = nullptr;
}


//---------------------------------------------------------------------------
void SpriteSheetButton::Activate()
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

	EventSystem::GetInstance()->FireEvent( eventName );
}


//---------------------------------------------------------------------------
void SpriteSheetButton::RenderIdle()
{
	if ( !m_idleMaterial ) return;

	m_idleMaterial->ActivateMaterial();
	RenderTexturedQuadFromSpriteSheetWithVertexArrayAndShader( m_idleTint, m_minTexCoords, m_maxTexCoords, m_idleMaterial->GetShaderProgramID(), V_QUADS );
}


//---------------------------------------------------------------------------
void SpriteSheetButton::RenderFocused()
{
	if ( !m_focusedMaterial ) return;

	m_focusedMaterial->ActivateMaterial();
	RenderTexturedQuadFromSpriteSheetWithVertexArrayAndShader( m_focusedTint, m_minTexCoords, m_maxTexCoords, m_focusedMaterial->GetShaderProgramID(), V_QUADS );
}


//---------------------------------------------------------------------------
void SpriteSheetButton::RenderPressed()
{
	if ( !m_pressedMaterial ) return;

	m_pressedMaterial->ActivateMaterial();
	RenderTexturedQuadFromSpriteSheetWithVertexArrayAndShader( m_pressedTint, m_minTexCoords, m_maxTexCoords, m_pressedMaterial->GetShaderProgramID(), V_QUADS );
}