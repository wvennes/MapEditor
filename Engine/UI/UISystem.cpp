//---------------------------------------------------------------------------
// UISystem.cpp
//---------------------------------------------------------------------------

#include "Engine/Core/EventSystem.hpp"
#include "Engine/Input/InputEventManager.hpp"
#include "Engine/UI/ContainerWidget.hpp"
#include "Engine/UI/UISystem.hpp"

#define STATIC

STATIC UISystem*	UISystem::s_instance = nullptr;


//---------------------------------------------------------------------------
STATIC UISystem* UISystem::GetInstance()
{
	if ( !s_instance ) s_instance = new UISystem();
	return s_instance;
}


//---------------------------------------------------------------------------
STATIC void UISystem::DestroyInstance()
{
	if ( s_instance ) delete s_instance;
	s_instance = nullptr;
}


//---------------------------------------------------------------------------
UISystem::UISystem()
	: m_rootWindow( nullptr )
{
	Initialize();
}


//---------------------------------------------------------------------------
UISystem::~UISystem()
{
	if ( m_rootWindow ) delete m_rootWindow;
}


//---------------------------------------------------------------------------
void UISystem::Initialize()
{
	m_rootWindow = new ContainerWidget( Vec2f( 0.f, 0.f ), InputEventManager::GetInstance()->GetWindowSize() );
}


//---------------------------------------------------------------------------
void UISystem::AddToWindow( Widget* widget )
{
	if ( !m_rootWindow ) return;

	m_rootWindow->AddChildWidget( widget );
}


//---------------------------------------------------------------------------
void UISystem::Update()
{
	InputEventManager* es = InputEventManager::GetInstance();

	Vec2f mousePosition = es->GetMouseCursorLocation();
	m_rootWindow->HandleMousePointerMovement( mousePosition );

	if ( es->HasMouseButtonBeenPressed( MOUSE_LBUTTON ) )
	{
		Event e( "mouseDown" );
		e.AddProperty< MouseButton >( "button", MOUSE_LBUTTON );
		m_rootWindow->HandleInputEvent( e );
	}
	else if ( es->HasMouseButtonBeenReleased( MOUSE_LBUTTON ) )
	{
		Event e( "mouseUp" );
		e.AddProperty< MouseButton >( "button", MOUSE_LBUTTON );
		m_rootWindow->HandleInputEvent( e );
	}
}


//---------------------------------------------------------------------------
void UISystem::RenderWidgets()
{
	m_rootWindow->Render();
}