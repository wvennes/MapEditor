//---------------------------------------------------------------------------
// TheEditor.cpp
//---------------------------------------------------------------------------

#include "Engine/Core/EventSystem.hpp"
#include "Engine/Core/TinyXMLHelper.hpp"
//#include "Engine/Input/InputManager.hpp"
#include "Engine/Input/InputEventManager.hpp"
#include "Engine/Math/Colors.hpp"
#include "Engine/Rendering/Material.hpp"
#include "Engine/Rendering/ShaderProgram.hpp"
#include "Engine/Rendering/BuiltInShaders.hpp"
#include "Engine/Rendering/TextRenderer.hpp"
#include "Engine/Rendering/VennRenderer.hpp"
#include "Engine/Rendering/Texture.hpp"
#include "Engine/UI/UISystem.hpp"
#include "Engine/UI/ButtonWidget.hpp"

#include "Editor/SheetLoader.hpp"
#include "Editor/SpriteSheetDisplay.hpp"
#include "Editor/SpriteSheetButton.hpp"
#include "Editor/SpriteSheetGridButton.hpp"
#include "Editor/TileGridWidget.hpp"
#include "Editor/XMLMapHandler.hpp"
#include "Editor/TheEditor.hpp"

#define UNUSED( x ) ( void ) ( x )

//extern InputManager* g_theInputManager;
extern VennRenderer* g_theRenderer;
extern TextRenderer* g_theTextRenderer;


//---------------------------------------------------------------------------
TheEditor::TheEditor()
	: m_mapWidget( nullptr )
	, m_sheetDisplayWidget( nullptr )
	, m_currentlySelectedTile( nullptr )
{
	RegisterEvents();
	InitializeMaterials();
	InitializeWidgets();
	//InitializeTestWidgets();
}


//---------------------------------------------------------------------------
TheEditor::~TheEditor()
{
	//if ( m_testWidget ) delete m_testWidget;
}


//---------------------------------------------------------------------------
void TheEditor::RegisterEvents()
{
	EventSystem* es = EventSystem::GetInstance();

	es->RegisterEvent( new Event( "nextPage" ) );
	es->RegisterEvent( new Event( "prevPage" ) );
	es->RegisterEvent( new Event( "reimport" ) );
	es->RegisterEvent( new Event( "selectTile" ) );
	es->RegisterEvent( new Event( "placeTile" ) );
	es->RegisterEvent( new Event( "export" ) );
	es->RegisterEvent( new Event( "import" ) );

	es->RegisterEventListener( "nextPage", this, &TheEditor::OnNextPage );
	es->RegisterEventListener( "prevPage", this, &TheEditor::OnPrevPage );
	es->RegisterEventListener( "reimport", this, &TheEditor::OnReimport );
	es->RegisterEventListener( "selectTile", this, &TheEditor::OnSelectTile );
	es->RegisterEventListener( "placeTile", this, &TheEditor::OnPlaceTile );
	es->RegisterEventListener( "export", this, &TheEditor::OnExport );
	es->RegisterEventListener( "import", this, &TheEditor::OnImport );
}


//---------------------------------------------------------------------------
void TheEditor::UnregisterEvents()
{
	EventSystem* es = EventSystem::GetInstance();
	es->UnregisterEventListenersForContext( "nextPage", this );
	es->UnregisterEventListenersForContext( "prevPage", this );
	es->UnregisterEventListenersForContext( "reimport", this );
	es->UnregisterEventListenersForContext( "selectTile", this );
	es->UnregisterEventListenersForContext( "placeTile", this );
}


//---------------------------------------------------------------------------
void TheEditor::InitializeTestWidgets()
{
//	EventSystem* es = EventSystem::GetInstance();
	UISystem* us = UISystem::GetInstance();
	ButtonWidget* buttonOne = new ButtonWidget( Vec2f( 800.f, 500.f ), Vec2f( 100.f, 100.f ) );
	buttonOne->SetIdleMaterial( "testIdle" );
	buttonOne->SetFocusedMaterial( "testFocused" );
	buttonOne->SetPressedMaterial( "testPressed" );

	ButtonWidget* buttonTwo = new ButtonWidget( Vec2f( 1000.f, 500.f ), Vec2f( 100.f, 100.f ) );
	buttonTwo->SetIdleMaterial( "testIdle" );
	buttonTwo->SetFocusedMaterial( "testFocused" );
	buttonTwo->SetPressedMaterial( "testPressed" );
	buttonTwo->SetLabelText( "PUSH ME", "./Data/Fonts/MSGothic.fnt", 20.f, Colors::WHITE );

	ButtonWidget* buttonTroll = new ButtonWidget( Vec2f( 600.f, 0.f ), Vec2f( 100.f, 100.f ) );
	buttonTroll->SetIdleMaterial( "testTroll" );
	buttonTroll->SetFocusedMaterial( "testTroll" );
	buttonTroll->SetPressedMaterial( "testTroll" );
	buttonTroll->SetLabelText( "PUSH ME", "./Data/Fonts/MSGothic.fnt", 20.f, Colors::WHITE );

// 	Event* testEvent = new Event( "widgetClick" );
// 	es->RegisterEvent( testEvent );
// 	es->RegisterEventListener( "widgetClick", &CalculateRandomInt );
// 
// 	Event* testClearEvent = new Event( "clearRand" );
// 	es->RegisterEvent( testClearEvent );
// 	es->RegisterEventListener( "clearRand", &ClearRandomInt );

	buttonOne->SetOnActivateEvent( "widgetClick" );
	buttonTwo->SetOnActivateEvent( "clearRand" );
	buttonTroll->SetOnActivateEvent( "shutdown" );

	us->AddToWindow( buttonOne );
	us->AddToWindow( buttonTwo );
	us->AddToWindow( buttonTroll );
}


//---------------------------------------------------------------------------
void TheEditor::InitializeWidgets()
{
	PlaceTileGridWidget();
	PlaceSpritesheetGridWidget();
	PlaceCurrentlySelectedTileWindow();
	PlaceButtons();
}


//---------------------------------------------------------------------------
void TheEditor::InitializeMaterials()
{
	Material* mat = Material::CreateAndGetMaterial( "testIdle", new ShaderProgram( *Material::GetDefaultUnlitMaterial()->GetShaderProgram() ) );
	mat->SetDiffuseTexture( Texture::CreateOrGetTexture( "./Data/Textures/Test/idletest.png" ) );

	mat = Material::CreateAndGetMaterial( "testFocused", new ShaderProgram( *Material::GetDefaultUnlitMaterial()->GetShaderProgram() ) );
	mat->SetDiffuseTexture( Texture::CreateOrGetTexture( "./Data/Textures/Test/hovertest.png" ) );

	mat = Material::CreateAndGetMaterial( "testPressed", new ShaderProgram( *Material::GetDefaultUnlitMaterial()->GetShaderProgram() ) );
	mat->SetDiffuseTexture( Texture::CreateOrGetTexture( "./Data/Textures/Test/pressedtest.png" ) );

	mat = Material::CreateAndGetMaterial( "testTroll", new ShaderProgram( *Material::GetDefaultUnlitMaterial()->GetShaderProgram() ) );
	mat->SetDiffuseTexture( Texture::CreateOrGetTexture( "./Data/Textures/Test/trolltest.png" ) );
}


//---------------------------------------------------------------------------
void TheEditor::PlaceTileGridWidget()
{
	///////////////////////////////
	float cellSize = 64.f;
	Vec2i tileDims( 13, 13 );
	Vec2f screenPosition( 50.f, 75.f );
	///////////////////////////////

	Vec2f widgetDims( ( float ) tileDims.x * cellSize, ( float ) tileDims.y * cellSize );

	UISystem* us = UISystem::GetInstance();
	//SpriteSheetDisplay* grid = new SpriteSheetDisplay();
	m_mapWidget = new TileGridWidget( screenPosition, widgetDims, tileDims, Colors::WHITE, Colors::CYAN, Colors::GREEN );
	m_mapWidget->SetActivateEvent( "placeTile" );
	us->AddToWindow( m_mapWidget );
}


//---------------------------------------------------------------------------
void TheEditor::PlaceSpritesheetGridWidget()
{
	///////////////////////////////
	float cellSize = 64.f;
	Vec2i tileDims( 8, 7 );
	Vec2f screenPosition( 1000.f, 375.f );
	///////////////////////////////

	Vec2f widgetDims( ( float ) tileDims.x * cellSize, ( float ) tileDims.y * cellSize );
	SheetLoader s;
	m_sheetDisplayWidget = new SpriteSheetDisplay();
	m_sheetDisplayWidget->SetScreenPosition( screenPosition );
	m_sheetDisplayWidget->SetDimensions( widgetDims );
	m_sheetDisplayWidget->SetWidgetTileDimensions( tileDims );
	m_sheetDisplayWidget->SetTints( Rgba( 120, 120, 120, 255 ), Colors::WHITE, Rgba( 120, 120, 120, 255 ) );
	//m_gridDisplayWidget->SetSpriteSheet( "./Data/Sheets/SimpleMinerAtlas.png", Vec2i( 32, 32 ) );
	s.ImportSheetsFromMetadata( "./Data/Sheets/*.sheetmeta.xml", m_sheetDisplayWidget );
	m_sheetDisplayWidget->SetActivateEvent( "selectTile" );
	UISystem::GetInstance()->AddToWindow( m_sheetDisplayWidget );
}


//---------------------------------------------------------------------------
void TheEditor::PlaceButtons()
{
	UISystem* us = UISystem::GetInstance();

	// exit and reimport buttons
	/////////////////////////////////////////
	Vec2f exitPos( 1425.f, 10.f );
	Vec2f reimportPos( 1250.f, 10.f );
	Vec2f exportPos( 10.f, 10.f );
	Vec2f importPos( 170.f, 10.f );
	Vec2f exitAndReimportDims( 150.f, 50.f );
	/////////////////////////////////////////

	// page buttons
	/////////////////////////////////////////
	Vec2f prevPos( 1175.f, 350.f );
	Vec2f nextPos( 1275.f, 350.f );
	Vec2f pageButtonDims( 75.f, 50.f );
	/////////////////////////////////////////

	// exit button
	ButtonWidget* exit = new ButtonWidget( exitPos, exitAndReimportDims );
	exit->SetLabelText( "EXIT", "./Data/Fonts/MSGothic.fnt", 32.f, Colors::WHITE );
	exit->SetOnActivateEvent( "shutdown" );
	exit->SetMaterials( "defaultUnlit", "defaultUnlit", "defaultUnlit" );
	exit->SetTints( Colors::GREY, Colors::WHITE, Colors::GREY );
	us->AddToWindow( exit );

	// reimport button
	ButtonWidget* reimport = new ButtonWidget( reimportPos, exitAndReimportDims );
	reimport->SetLabelText( "RELOAD", "./Data/Fonts/MSGothic.fnt", 32.f, Colors::WHITE );
	reimport->SetMaterials( "defaultUnlit", "defaultUnlit", "defaultUnlit" );
	reimport->SetTints( Colors::GREY, Colors::WHITE, Colors::GREY );
	reimport->SetOnActivateEvent( "reimport" );
	us->AddToWindow( reimport );

	// export button
	ButtonWidget* exportButton = new ButtonWidget( exportPos, exitAndReimportDims );
	exportButton->SetLabelText( "EXPORT", "./Data/Fonts/MSGothic.fnt", 32.f, Colors::WHITE );
	exportButton->SetMaterials( "defaultUnlit", "defaultUnlit", "defaultUnlit" );
	exportButton->SetTints( Colors::GREY, Colors::WHITE, Colors::GREY );
	exportButton->SetOnActivateEvent( "export" );
	us->AddToWindow( exportButton );

	// import button
	ButtonWidget* importButton = new ButtonWidget( importPos, exitAndReimportDims );
	importButton->SetLabelText( "IMPORT", "./Data/Fonts/MSGothic.fnt", 32.f, Colors::WHITE );
	importButton->SetMaterials( "defaultUnlit", "defaultUnlit", "defaultUnlit" );
	importButton->SetTints( Colors::GREY, Colors::WHITE, Colors::GREY );
	importButton->SetOnActivateEvent( "import" );
	us->AddToWindow( importButton );

	// prev page button
	ButtonWidget* prev = new ButtonWidget( prevPos, pageButtonDims );
	prev->SetLabelText( "PREV", "./Data/Fonts/MSGothic.fnt", 32.f, Colors::WHITE );
	prev->SetMaterials( "defaultUnlit", "defaultUnlit", "defaultUnlit" );
	prev->SetTints( Colors::GREY, Colors::WHITE, Colors::GREY );
	prev->SetOnActivateEvent( "prevPage" );
	us->AddToWindow( prev );

	// next page button
	ButtonWidget* next = new ButtonWidget( nextPos, pageButtonDims );
	next->SetLabelText( "NEXT", "./Data/Fonts/MSGothic.fnt", 32.f, Colors::WHITE );
	next->SetMaterials( "defaultUnlit", "defaultUnlit", "defaultUnlit" );
	next->SetTints( Colors::GREY, Colors::WHITE, Colors::GREY );
	next->SetOnActivateEvent( "nextPage" );
	us->AddToWindow( next );
}


//---------------------------------------------------------------------------
void TheEditor::PlaceCurrentlySelectedTileWindow()
{
	/////////////////////////////////////////
	Vec2f tileDims( 128.f, 128.f );
	Vec2f screenPos( 1200.f, 200.f );
	/////////////////////////////////////////

	m_currentlySelectedTile = new SpriteSheetButton();
	m_currentlySelectedTile->SetMaterials( "defaultUnlit", "defaultUnlit", "defaultUnlit" );
	m_currentlySelectedTile->SetDimensions( tileDims );
	m_currentlySelectedTile->SetScreenPosition( screenPos );
	UISystem::GetInstance()->AddToWindow( m_currentlySelectedTile );
}


//---------------------------------------------------------------------------
void TheEditor::OnNextPage( Event& e )
{
	UNUSED( e );
	m_sheetDisplayWidget->NextPage();
}


//---------------------------------------------------------------------------
void TheEditor::OnPrevPage( Event& e )
{
	UNUSED( e );
	m_sheetDisplayWidget->PreviousPage();
}


//---------------------------------------------------------------------------
void TheEditor::OnReimport( Event& e )
{
	UNUSED( e );
	SheetLoader s;
	s.ImportSheetsFromMetadata( "./Data/Sheets/*.sheetmeta.xml", m_sheetDisplayWidget );
	m_sheetDisplayWidget->SetActivateEvent( "selectTile" );
	//m_gridDisplayWidget->SetSpriteSheet( "./Data/Sheets/SimpleMinerAtlas.png", Vec2i( 32, 32 ) );
}


//---------------------------------------------------------------------------
void TheEditor::OnSelectTile( Event& e )
{
	Material* mat = nullptr;
	SpriteSheet* sheet = nullptr;
	Vec2f minTexCoords;
	Vec2f maxTexCoords;
	Vec2i sheetCoords;
	std::string sheetName;
	if ( !e.GetProperty( "material", mat ) ) return;
	if ( !e.GetProperty( "sheet", sheet ) ) return;
	if ( !e.GetProperty( "minTexCoords", minTexCoords ) ) return;
	if ( !e.GetProperty( "maxTexCoords", maxTexCoords ) ) return;
	if ( !e.GetProperty( "sheetCoords", sheetCoords ) ) return;
	if ( !e.GetProperty( "sheetName", sheetName ) ) return;

	if ( m_currentlySelectedTile->GetSpriteSheet() ) delete m_currentlySelectedTile->GetSpriteSheet();

	m_currentlySelectedTile->SetMaterials( mat, mat, mat );
	m_currentlySelectedTile->SetMinTexCoords( minTexCoords );
	m_currentlySelectedTile->SetMaxTexCoords( maxTexCoords );
	m_currentlySelectedTile->SetSheetCoords( sheetCoords );
	m_currentlySelectedTile->SetSheetName( sheetName );
	m_currentlySelectedTile->SetSpriteSheet( new SpriteSheet( *sheet ) );
}


//---------------------------------------------------------------------------
void TheEditor::OnPlaceTile( Event& e )
{
	Material* mat = m_currentlySelectedTile->GetIdleMaterial();
	Vec2f minTexCoords = m_currentlySelectedTile->GetMinTexCoords();
	Vec2f maxTexCoords = m_currentlySelectedTile->GetMaxTexCoords();
	SpriteSheetGridButton* selectedGridTile = nullptr;

	if ( !e.GetProperty( "context", selectedGridTile ) ) return;

	selectedGridTile->ClearSpriteSheet();

	selectedGridTile->SetMaterials( mat, mat, mat );
	selectedGridTile->SetMinTexCoords( minTexCoords );
	selectedGridTile->SetMaxTexCoords( maxTexCoords );
	selectedGridTile->SetSheetName( m_currentlySelectedTile->GetSheetName() );
	selectedGridTile->SetSheetCoords( m_currentlySelectedTile->GetSheetCoords() );
	if ( m_currentlySelectedTile->GetSpriteSheet() ) selectedGridTile->SetSpriteSheet( new SpriteSheet( *m_currentlySelectedTile->GetSpriteSheet() ) );
}


//---------------------------------------------------------------------------
void TheEditor::OnExport( Event& e )
{
	UNUSED( e );

	XMLMapHandler handler;
	handler.SaveFileFromWidget( "./Data/saved.map.xml", m_mapWidget );
}


//---------------------------------------------------------------------------
void TheEditor::OnImport( Event& e )
{
	UNUSED( e );

	XMLMapHandler handler;
	handler.LoadFileToWidget( "./Data/saved.map.xml", m_mapWidget );
}


//---------------------------------------------------------------------------
bool TheEditor::ProcessInput()
{
	return false;
}


//---------------------------------------------------------------------------
bool TheEditor::Update()
{
	UISystem::GetInstance()->Update();
	return false;
}


//---------------------------------------------------------------------------
void TheEditor::Render()
{
	g_theRenderer->ApplyOrthoProjection( 0.f, 1600.f, 0.f, 900.f, 0.f, 1.f );
	UISystem::GetInstance()->RenderWidgets();
}