#include "boost/python.hpp"

#include "GafferBindings/PlugBinding.h"
#include "GafferBindings/SignalBinding.h"
#include "Gaffer/Plug.h"
#include "Gaffer/Node.h"

#include "IECore/bindings/Wrapper.h"
#include "IECore/bindings/WrapperToPython.h"
#include "IECore/bindings/IntrusivePtrPatch.h"

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

void GafferBindings::bindPlug()
{
	typedef class_<Plug, PlugPtr, boost::noncopyable, bases<GraphComponent> > PlugPyClass;

	PlugPyClass( "Plug" )
		.def( "node", (NodePtr (Plug::*)())&Plug::node )
		.def( "acceptsInput", &Plug::acceptsInput )
		.def( "setInput", (void (Plug::*)(PlugPtr))&Plug::setInput )
		.def( "getInput", (PlugPtr (Plug::*)())&Plug::getInput<Plug> )
		.def( "outputs", &outputs )
	;

	INTRUSIVE_PTR_PATCH( Plug, PlugPyClass );
	
	implicitly_convertible<PlugPtr, GraphComponentPtr>();
	implicitly_convertible<PlugPtr, ConstPlugPtr>();
	
}
