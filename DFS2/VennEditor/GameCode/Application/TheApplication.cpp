//---------------------------------------------------------------------------
// TheApplication.cpp
//---------------------------------------------------------------------------

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "Engine/Rendering/VennRenderer.hpp"
#include "Engine/Rendering/Texture.hpp"
#include "Engine/Rendering/TextRenderer.hpp"
#include "Engine/Rendering/Shader.hpp"
#include "Engine/Rendering/Material.hpp"
//#include "Engine/Input/InputManager.hpp"
#include "Engine/Input/InputEventManager.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Math/Colors.hpp"
#include "Engine/Core/Time.hpp"
#include "Engine/UI/UISystem.hpp"
#include "ENgine/Rendering/BitmapFont.hpp"

#include "../Editor/TheEditor.hpp"
#include "../Application/TheApplication.hpp"

#include "Engine/Core/EventSystem.hpp"

#define UNUSED( x ) ( void ) ( x )

TextRenderer* g_theTextRenderer = nullptr;


//---------------------------------------------------------------------------
TheApplication::TheApplication( void* platformHandler, void* displayDeviceContext )
	: m_platformHandler( platformHandler )
	, m_displayDeviceContext( ( HDC ) displayDeviceContext )
	, m_theEditor( nullptr )
{
	Initialize();
}


//---------------------------------------------------------------------------
TheApplication::~TheApplication()
{
	delete g_theRenderer;
	delete g_theTextRenderer;
	//delete g_theInputManager;
	InputEventManager::DestroyInstance();
	EventSystem::DestroyInstance();
	UISystem::DestroyInstance();

	Texture::FreeTextureRegistryMemory();
	Shader::FreeShaderRegistryMemory();
	Material::FreeMaterialRegistryMemory();
	BitmapFont::FreeBitmapFontRegistryMemory();

	delete m_theEditor;
}


//---------------------------------------------------------------------------
void TheApplication::Initialize()
{
	g_theRenderer = new VennRenderer();
	g_theTextRenderer = new TextRenderer( "Data/Fonts/BookAntiqua.fnt" );
	//g_theInputManager = new InputManager( m_platformHandler );
	InputEventManager::InitializeManager( m_platformHandler );
	RegisterEventListeners();

	// let's make our lines smooth...
	g_theRenderer->VennLineWidth( 8.f );
	g_theRenderer->VennPointSize( 15.f );

	// allow us to have translucent pixels
	g_theRenderer->VennEnable( V_BLEND );
	g_theRenderer->VennBlendFunc( V_SRC_ALPHA, V_ONE_MINUS_SRC_ALPHA );

	// enable depth tests
	//g_theRenderer->VennEnable( V_DEPTH_TEST );
// 	g_theRenderer->VennEnable( V_CULL_FACE );

	// enable line smoothing
	g_theRenderer->VennEnable( V_LINE_SMOOTH );
	g_theRenderer->VennHint( V_LINE_SMOOTH_HINT, V_DONT_CARE );

	g_theRenderer->VennClearStencil( 0 );

	Material::GenerateDefaultMaterials();

	m_theEditor = new TheEditor();
}


//---------------------------------------------------------------------------
void TheApplication::RegisterEventListeners()
{
	EventSystem::GetInstance()->RegisterEventListener( "shutdown", this, &TheApplication::OnShutdown );
}


//---------------------------------------------------------------------------
void TheApplication::Run()
{
	while ( !g_isQuitting && !g_isGameOver )
	{
		RunFrame();
	}
}


//---------------------------------------------------------------------------
void TheApplication::RunFrame()
{
	ProcessInput();
	Update();
	Render();
}


//---------------------------------------------------------------------------
void TheApplication::ProcessInput()
{
	//g_isQuitting = g_theInputManager->IsKeyDown( Input::KEY_ENGINE_SHUTDOWN );
	if ( m_theEditor ) m_theEditor->ProcessInput();
}


//---------------------------------------------------------------------------
void TheApplication::OnShutdown( Event& e )
{
	UNUSED( e );
	g_isQuitting = true;
}


//---------------------------------------------------------------------------
void TheApplication::Update()
{
	//g_theInputManager->Update();
	InputEventManager::GetInstance()->Update();
	if ( m_theEditor ) m_theEditor->Update();
}


//---------------------------------------------------------------------------
void TheApplication::Render()
{
	// clear screen to black
	g_theRenderer->VennClearColor( Colors::BLACK );
	g_theRenderer->VennClearDepth( 1.f );
	// clear the screen
	g_theRenderer->VennClear( V_COLOR_BUFFER_BIT | V_DEPTH_BUFFER_BIT | V_STENCIL_BUFFER_BUT );

	g_theRenderer->UpdateDebugDrawList();

	if ( m_theEditor ) m_theEditor->Render();

	// swap buffers
	SwapBuffers( ( HDC ) m_displayDeviceContext );

	// use custom Time.h to limit our fps
	LimitFPS();
}