#include "boost/python.hpp"

#include "GafferBindings/PlugBinding.h"
#include "Gaffer/Plug.h"
#include "Gaffer/Node.h"

#include "IECore/bindings/RunTimeTypedBinding.h"

using namespace boost::python;
using namespace GafferBindings;
using namespace Gaffer;

std::string GafferBindings::serialisePlugDirection( Plug::Direction direction )
{
	switch( direction )
	{
		case Plug::In :
			return "Gaffer.Plug.Direction.In";
		case Plug::Out :
			return "Gaffer.Plug.Direction.Out";
		default :
			return "Gaffer.Plug.Direction.Invalid";
	}
}

std::string GafferBindings::serialisePlugFlags( unsigned flags )
{
	if( flags )
	{
		return "Gaffer.Plug.Flags.Dynamic";
	}
	return "Gaffer.Plug.Flags.None";
}

static boost::python::tuple outputs( Plug &p )
{
	const Plug::OutputContainer &o = p.outputs();
	boost::python::list l;
	for( Plug::OutputContainer::const_iterator it=o.begin(); it!=o.end(); it++ )
	{
		l.append( PlugPtr( *it ) );
	}
	return boost::python::tuple( l );
}

static PlugPtr constructor( const std::string &name, Plug::Direction direction, Plug::Flags flags )
{
	return new Plug( name, direction, flags );
}

void GafferBindings::bindPlug()
{

	IECore::RunTimeTypedClass<Plug> c;
	{
		scope s( c );
		enum_<Plug::Direction>( "Direction" )
			.value( "Invalid", Plug::Invalid )
			.value( "In", Plug::In )
			.value( "Out", Plug::Out )
		;
		enum_<Plug::Flags>( "Flags" )
			.value( "None", Plug::None )
			.value( "Dynamic", Plug::Dynamic )
			.value( "All", Plug::All )
		;
	}
	
	c.def( "__init__", make_constructor(
				&constructor,
				default_call_policies(),
				(
					boost::python::arg_( "name" )=Plug::staticTypeName(),
					boost::python::arg_( "direction" )=Plug::In,
					boost::python::arg_( "flags" )=Plug::None
				)
			)
		)
		.def( "node", (NodePtr (Plug::*)())&Plug::node )
		.def( "direction", &Plug::direction )
		.def( "getFlags", (unsigned (Plug::*)() const )&Plug::getFlags )
		.def( "getFlags", (bool (Plug::*)( unsigned ) const )&Plug::getFlags )
		.def( "setFlags", (void (Plug::*)( unsigned ) )&Plug::setFlags )
		.def( "setFlags", (void (Plug::*)( unsigned, bool ) )&Plug::setFlags )
		.def( "acceptsInput", &Plug::acceptsInput )
		.def( "setInput", (void (Plug::*)(PlugPtr))&Plug::setInput )
		.def( "getInput", (PlugPtr (Plug::*)())&Plug::getInput<Plug> )
		.def( "removeOutputs", &Plug::removeOutputs )
		.def( "outputs", &outputs )
	;
	
}
