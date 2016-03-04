//---------------------------------------------------------------------------
// TinyXMLHelper.cpp
//---------------------------------------------------------------------------

#include "Engine/Core/TinyXMLHelper.hpp"


//---------------------------------------------------------------------------
unsigned char GetAttributeAsUnsignedChar( TiXmlElement* element, const char* name )
{
	const char* value = element->Attribute( name );
	if ( value == nullptr ) return 0;
	return ( unsigned char ) *value; // first index of return value
}


//---------------------------------------------------------------------------
char GetAttributeAsChar( TiXmlElement* element, const char* name )
{
	const char* value = element->Attribute( name );
	if ( value == nullptr ) return 0;
	return ( char ) *value;
}


//---------------------------------------------------------------------------
unsigned int GetAttributeAsUnsignedInt( TiXmlElement* element, const char* name )
{
	const char* value = element->Attribute( name );
	if ( value == nullptr ) return 0;
	return ( unsigned int ) ( atoi( value ) );
}


//---------------------------------------------------------------------------
int GetAttributeAsInt( TiXmlElement* element, const char* name )
{
	const char* value = element->Attribute( name );
	if ( value == nullptr ) return 0;
	return ( int ) ( atoi( value ) );
}


//---------------------------------------------------------------------------
float GetAttributeAsFloat( TiXmlElement* element, const char* name )
{
	const char* value = element->Attribute( name );
	if ( value == nullptr ) return 0;
	return ( float ) ( atof( value ) );
}


//---------------------------------------------------------------------------
std::string GetAttributeAsString( TiXmlElement* element, const char* name )
{
	const char* value = element->Attribute( name );
	if ( value == nullptr ) return "null";
	return std::string( value );
}


// this function assumes you separate your value with a comma, like so: value="0,0"
//---------------------------------------------------------------------------
Vec2i GetAttributeAsVec2i( TiXmlElement* element, const char* name )
{
	const char* value = element->Attribute( name );
	if ( value == nullptr ) return Vec2i( 0, 0 );

	Vec2i result( 0, 0 );
	sscanf_s( value, "%d,%d", &result.x, &result.y );
	return result;
}


//---------------------------------------------------------------------------
Vec2f GetAttributeAsVec2f( TiXmlElement* element, const char* name )
{
	const char* value = element->Attribute( name );
	if ( value == nullptr ) return Vec2f( 0.f, 0.f );

	Vec2f result( 0, 0 );
	sscanf_s( value, "%f,%f", &result.x, &result.y );
	return result;
}