//---------------------------------------------------------------------------
// Properties.hpp -- generic property map for any data type
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_PROPERTIES_
#define _INCLUDED_PROPERTIES_

#include <map>
#include <string>

#include "Engine/Core/StringTable.hpp"

class AbstractProperty;

typedef std::map< unsigned int, AbstractProperty* >	PropertyMap;


/////////////////////////////////////////////////////////////////////////////
class AbstractProperty
{
public:
	AbstractProperty() {}
	virtual ~AbstractProperty() {}
};


/////////////////////////////////////////////////////////////////////////////
template < typename T >
class Property : public AbstractProperty
{
public:
	T	m_property;

public:
	Property( const T& property ) : AbstractProperty(), m_property( property ) {}

};

/////////////////////////////////////////////////////////////////////////////
class Properties
{
public:
	Properties() {}
	~Properties() { DestroyMap( m_propertyMap ); }

	template < typename T >
	bool				GetProperty( const std::string& name, T& reference );
	template < typename T >
	bool				ChangeProperty( const std::string& name, const T& reference );
	template < typename T >
	bool				AddOrChangeProperty( const std::string& name, const T& reference );
	template < typename T >
	bool				AddProperty( const std::string& name, const T& reference );

	//const PropertyMap*	GetPropertyMap() const { return &m_propertyMap; }

private:
	PropertyMap			m_propertyMap;

};


//---------------------------------------------------------------------------
template < typename T >
bool Properties::GetProperty( const std::string& name, T& reference )
{
	unsigned int nameID = StringID( name );
	if ( m_propertyMap.find( nameID ) == m_propertyMap.end() ) return false;

	AbstractProperty* p = m_propertyMap[ nameID ];

	if ( Property< T >* derived = dynamic_cast< Property< T >* >( p ) )
	{
		reference = derived->m_property;
		return true;
	}

	return false;
}


//---------------------------------------------------------------------------
template < typename T >
bool Properties::ChangeProperty( const std::string& name, const T& reference )
{
	unsigned int nameID = StringID( name );
	if ( m_propertyMap.find( nameID ) == m_propertyMap.end() ) return false;

	AbstractProperty* p = m_propertyMap[ nameID ];

	if ( Property< T >* derived = dynamic_cast< Property< T >* > ( p ) )
	{
		derived->m_property = reference;
		return true;
	}

	return false;
}


//---------------------------------------------------------------------------
template < typename T >
bool Properties::AddOrChangeProperty( const std::string& name, const T& reference )
{
	if ( !ChangeProperty< T >( name, reference ) )
	{
		return AddProperty< T >( name, reference );
	}
	
	return false;
}


//---------------------------------------------------------------------------
template < typename T >
bool Properties::AddProperty( const std::string& name, const T& reference )
{
	unsigned int nameID = StringID( name );
	if ( m_propertyMap.find( nameID ) != m_propertyMap.end() ) return false;

	Property< T >* p = new Property< T >( reference );
	m_propertyMap.insert( std::make_pair( nameID, p ) );
	return true;
}

#endif