#include "boost/python.hpp"
#include "boost/format.hpp"

#include "GafferBindings/GraphComponentBinding.h"
#include "GafferBindings/SignalBinding.h"
#include "Gaffer/GraphComponent.h"

#include "IECore/bindings/IntrusivePtrPatch.h"

using namespace boost::python;
using namespace GafferBindings;
using namespace Gaffer;

static boost::python::tuple children( GraphComponent &c )
{
	const GraphComponent::ChildContainer &ch = c.children();
	boost::python::list l;
	for( GraphComponent::ChildContainer::const_iterator it=ch.begin(); it!=ch.end(); it++ )
	{
		l.append( *it );
	}
	return boost::python::tuple( l );
}

static object setAttr( object &self, const char *n, object c )
{
	GraphComponent &g = extract<GraphComponent &>( self );
	// we won't allow any attribute assignment to mask an existing GraphComponent child
	if( g.getChild<GraphComponent>( n ) )
	{
		std::string error = boost::str( boost::format( "\"%s\" already has a child named \"%s\"." ) % g.getName() % n );
		PyErr_SetString( PyExc_NameError, error.c_str() );
		throw_error_already_set();
	}

	extract<GraphComponentPtr> e( c );
	if( e.check() )
	{
		// add as a GraphComponent child
		GraphComponentPtr gc = e();
		gc->setName( n );
		g.addChild( gc );
	}
	else
	{
		// add as a normal attribute
		dict d = extract<dict>( self.attr( "__dict__" ) );
		d[n] = c;
	}

	return c;
}

struct UnarySlotCaller
{
	boost::signals::detail::unusable operator()( boost::python::object slot, GraphComponentPtr g )
	{
		slot( g );
		return boost::signals::detail::unusable();
	}
};

struct BinarySlotCaller
{

	boost::signals::detail::unusable operator()( boost::python::object slot, GraphComponentPtr g, GraphComponentPtr gg )
	{
		slot( g, gg );
		return boost::signals::detail::unusable();
	}
};

void GafferBindings::bindGraphComponent()
{
	typedef class_<GraphComponent, GraphComponentPtr, boost::noncopyable, bases<IECore::RunTimeTyped> > GraphComponentPyClass;

	scope s = GraphComponentPyClass( "GraphComponent" )
		.def( init<const std::string &>() )
		.def( "setName", &GraphComponent::setName, return_value_policy<copy_const_reference>() )
		.def( "getName", &GraphComponent::getName, return_value_policy<copy_const_reference>() )
		.def( "fullName", &GraphComponent::fullName )
		.def( "nameChangedSignal", &GraphComponent::nameChangedSignal, return_internal_reference<1>() )
		.def( "acceptsChild", &GraphComponent::acceptsChild )
		.def( "acceptsParent", &GraphComponent::acceptsParent )
		.def( "addChild", &GraphComponent::addChild )
		.def( "removeChild", &GraphComponent::removeChild )
		.def( "getChild", (GraphComponentPtr (GraphComponent::*)( const std::string & ))&GraphComponent::getChild<GraphComponent> )
		.def( "__getattr__", (GraphComponentPtr (GraphComponent::*)( const std::string & ))&GraphComponent::getChild<GraphComponent> )
		.def( "__setattr__", setAttr )
		.def( "children", &children )
		.def( "parent", (GraphComponentPtr (GraphComponent::*)())&GraphComponent::parent<GraphComponent> )
		.def( "childAddedSignal", &GraphComponent::childAddedSignal, return_internal_reference<1>() )
		.def( "childRemovedSignal", &GraphComponent::childRemovedSignal, return_internal_reference<1>() )
		.def( "parentChangedSignal", &GraphComponent::parentChangedSignal, return_internal_reference<1>() )
	;
	
	SignalBinder<GraphComponent::UnarySignal, DefaultSignalCaller<GraphComponent::UnarySignal>, UnarySlotCaller>::bind( "UnarySignal" );
	SignalBinder<GraphComponent::BinarySignal, DefaultSignalCaller<GraphComponent::BinarySignal>, BinarySlotCaller>::bind( "BinarySignal" );
		
	INTRUSIVE_PTR_PATCH( GraphComponent, GraphComponentPyClass );
	
	implicitly_convertible<GraphComponentPtr, IECore::RunTimeTypedPtr>();
	implicitly_convertible<GraphComponentPtr, ConstGraphComponentPtr>();

}
