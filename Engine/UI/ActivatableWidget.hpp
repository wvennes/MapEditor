//---------------------------------------------------------------------------
// ActivatableWidget.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_ACTIVATABLEWIDGET_
#define _INCLUDED_ACTIVATABLEWIDGET_

#include "Engine/Math/Colors.hpp"
#include "Engine/UI/Widget.hpp"

/////////////////////////////////////////////////////////////////////////////
class ActivatableWidget : public Widget
{
public:
	ActivatableWidget();
	ActivatableWidget( const Vec2f& screenCoords );
	ActivatableWidget( const Vec2f& screenCoords, const Vec2f& dimensions );
	ActivatableWidget( const ActivatableWidget& otherWidget );
	~ActivatableWidget();

	virtual void		Activate();
	virtual void		HandleInputEvent( Event& e );
	virtual void		Render();

	void				SetOnActivateEvent( const std::string& eventName );
	void				SetMaterials( const std::string& idle, const std::string& focused, const std::string& pressed );
	void				SetMaterials( Material* idle, Material* focused, Material* pressed );
	void				SetTints( const Rgba& idle, const Rgba& focused, const Rgba& pressed );
	void				SetIdleMaterial( const std::string& name );
	void				SetFocusedMaterial( const std::string& name );
	void				SetPressedMaterial( const std::string& name );
	void				SetIdleTint( const Rgba& color ) { m_idleTint = color; }
	void				SetFocusedTint( const Rgba& color ) { m_focusedTint = color; }
	void				SetPressedTint( const Rgba& color ) { m_pressedTint = color; }
	Rgba				GetIdleTint() const { return m_idleTint; }
	Rgba				GetFocusedTint() const { return m_focusedTint; }
	Rgba				GetPressedTint() const { return m_pressedTint; }
	Material*			GetIdleMaterial() { return m_idleMaterial; }
	Material*			GetFocusedMaterial() { return m_focusedMaterial; }
	Material*			GetPressedMaterial() { return m_pressedMaterial; }

protected:
	Material*			m_idleMaterial;
	Material*			m_focusedMaterial;
	Material*			m_pressedMaterial;
	Rgba				m_idleTint;
	Rgba				m_focusedTint;
	Rgba				m_pressedTint;

protected:
	virtual void		CreateEventProperties();
	virtual void		RenderIdle();
	virtual void		RenderFocused();
	virtual void		RenderPressed();

};

#endif