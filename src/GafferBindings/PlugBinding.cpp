#include "boost/python.hpp"

#include "GafferBindings/PlugBinding.h"
#include "GafferBindings/SignalBinding.h"
#include "Gaffer/Plug.h"
#include "Gaffer/Node.h"

#include "IECore/bindings/Wrapper.h"
#include "IECore/bindings/WrapperToPython.h"
#include "IECore/bindings/IntrusivePtrPatch.h"
#include "IECore/bindings/RunTimeTypedBinding.h"

using namespace boost::python;
using namespace GafferBindings;
using namespace Gaffer;

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

static PlugPtr constructor( const std::string &name, Plug::Direction direction )
{
	return new Plug( name, direction );
}

void GafferBindings::bindPlug()
{
	typedef class_<Plug, PlugPtr, boost::noncopyable, bases<GraphComponent> > PlugPyClass;

	PlugPyClass c( "Plug" );
	{
		scope s( c );
		enum_<Plug::Direction>( "Direction" )
			.value( "Invalid", Plug::Invalid )
			.value( "In", Plug::In )
			.value( "Out", Plug::Out )
		;
	}
	
	c.def( "__init__", make_constructor(
				&constructor,
				default_call_policies(),
				(
					boost::python::arg_( "name" )=Plug::staticTypeName(),
					boost::python::arg_( "direction" )=Plug::In
				)
			)
		)
		.def( "node", (NodePtr (Plug::*)())&Plug::node )
		.def( "direction", &Plug::direction )
		.def( "acceptsInput", &Plug::acceptsInput )
		.def( "setInput", (void (Plug::*)(PlugPtr))&Plug::setInput )
		.def( "getInput", (PlugPtr (Plug::*)())&Plug::getInput<Plug> )
		.def( "outputs", &outputs )
		.IE_COREPYTHON_DEFRUNTIMETYPEDSTATICMETHODS( Plug )
	;

	INTRUSIVE_PTR_PATCH( Plug, PlugPyClass );
	
	implicitly_convertible<PlugPtr, GraphComponentPtr>();
	implicitly_convertible<PlugPtr, ConstPlugPtr>();
	
}
