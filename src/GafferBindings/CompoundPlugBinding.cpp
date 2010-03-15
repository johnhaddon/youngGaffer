#include "boost/python.hpp"

#include "GafferBindings/CompoundPlugBinding.h"
#include "GafferBindings/Serialiser.h"
#include "GafferBindings/PlugBinding.h"
#include "Gaffer/CompoundPlug.h"

#include "IECorePython/RunTimeTypedBinding.h"

using namespace boost::python;
using namespace GafferBindings;
using namespace Gaffer;

static std::string serialise( Serialiser &s, ConstGraphComponentPtr g )
{
	ConstCompoundPlugPtr plug = IECore::staticPointerCast<const CompoundPlug>( g );
	std::string result = s.modulePath( g ) + "." + g->typeName() + "( \"" + g->getName() + "\", ";
	
	if( plug->direction()!=Plug::In )
	{
		result += "direction = " + serialisePlugDirection( plug->direction() ) + ", ";
	}
		
	if( plug->getFlags() )
	{
		result += "flags = " + serialisePlugFlags( plug->getFlags() ) + ", ";
	}
	
	if( plug->children().size() )
	{
		result += "children = ( ";
	
		PlugIterator pIt( plug->children().begin(), plug->children().end() );
		while( pIt!=plug->children().end() )
		{
			result += s.serialiseC( *pIt++ ) + ", ";
		}
	
		result += " )";
	}
		
	result += " )";

	return result;
}

static CompoundPlugPtr construct(
	const char *name,
	Plug::Direction direction,
	unsigned flags,
	tuple children
)
{
	CompoundPlugPtr result = new CompoundPlug( name, direction, flags );
	size_t s = extract<size_t>( children.attr( "__len__" )() );
	for( size_t i=0; i<s; i++ )
	{
		PlugPtr c = extract<PlugPtr>( children[i] );
		result->addChild( c );
	}
	return result;
}

void GafferBindings::bindCompoundPlug()
{
	IECorePython::RunTimeTypedClass<CompoundPlug>()
		.def( "__init__", make_constructor( construct, default_call_policies(),
				(
					boost::python::arg_( "name" )=CompoundPlug::staticTypeName(),
					boost::python::arg_( "direction" )=Plug::In,
					boost::python::arg_( "flags" )=Plug::None,
					boost::python::arg_( "children" )=tuple()
				)
			)
		)
	;
	
	Serialiser::registerSerialiser( CompoundPlug::staticTypeId(), serialise );
}
