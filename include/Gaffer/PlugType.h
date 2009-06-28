#ifndef GAFFER_PLUGTYPE_H
#define GAFFER_PLUGTYPE_H

#include "Gaffer/NumericPlug.h"
#include "Gaffer/CompoundNumericPlug.h"
#include "Gaffer/TypedPlug.h"

namespace Gaffer
{

/// This structure can be used to determine the appropriate Plug subclass
/// to use to store a value of type T.
template<typename T>
struct PlugType
{
	typedef void Type;
};

#define GAFFER_PLUGTYPE_SPECIALISE( VALUETYPE, PLUGTYPE ) 	\
	template<>												\
	struct PlugType<VALUETYPE>								\
	{														\
		typedef PLUGTYPE Type;								\
	};														\

GAFFER_PLUGTYPE_SPECIALISE( float, FloatPlug )
GAFFER_PLUGTYPE_SPECIALISE( int, IntPlug )

GAFFER_PLUGTYPE_SPECIALISE( Imath::V2f, V2fPlug )
GAFFER_PLUGTYPE_SPECIALISE( Imath::V3f, V3fPlug )

GAFFER_PLUGTYPE_SPECIALISE( Imath::V2i, V2iPlug )
GAFFER_PLUGTYPE_SPECIALISE( Imath::V3i, V3iPlug )

GAFFER_PLUGTYPE_SPECIALISE( Imath::Color3f, Color3fPlug )
GAFFER_PLUGTYPE_SPECIALISE( Imath::Color4f, Color4fPlug )

GAFFER_PLUGTYPE_SPECIALISE( std::string, StringPlug )

GAFFER_PLUGTYPE_SPECIALISE( Imath::M33f, M33fPlug )
GAFFER_PLUGTYPE_SPECIALISE( Imath::M44f, M44fPlug )


} // namespace Gaffer

#endif // GAFFER_PLUGTYPE_H
