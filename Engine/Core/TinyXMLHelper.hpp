//---------------------------------------------------------------------------
// TinyXMLHelper.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_TINYXMLHELPER_
#define _INCLUDED_TINYXMLHELPER_

#include "Engine/External/TinyXML/tinyxml.h"
#include "Engine/Math/Vec2.hpp"

#include <string>

unsigned char	GetAttributeAsUnsignedChar( TiXmlElement* element, const char* name );
char			GetAttributeAsChar( TiXmlElement* element, const char* name );
unsigned int	GetAttributeAsUnsignedInt( TiXmlElement* element, const char* name );
int				GetAttributeAsInt( TiXmlElement* element, const char* name );
float			GetAttributeAsFloat( TiXmlElement* element, const char* name );
std::string		GetAttributeAsString( TiXmlElement* element, const char* name );
Vec2i			GetAttributeAsVec2i( TiXmlElement* element, const char* name );
Vec2f			GetAttributeAsVec2f( TiXmlElement* element, const char* name );

#endif