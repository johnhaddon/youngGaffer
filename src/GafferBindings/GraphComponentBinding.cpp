#include "boost/python.hpp"
#include "boost/format.hpp"

#include "GafferBindings/GraphComponentBinding.h"
#include "GafferBindings/SignalBinding.h"
#include "Gaffer/GraphComponent.h"

#include "IECore/bindings/IntrusivePtrPatch.h"
#include "IECore/bindings/RunTimeTypedBinding.h"

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

/// \todo Document the different semantics of addChild and setItem in a scripting
/// reference somewhere.
static void setItem( GraphComponent &g, const char *n, GraphComponentPtr c )
{
	GraphComponentPtr existingChild = g.getChild<GraphComponent>( n );
	if( existingChild )
	{
		g.removeChild( existingChild );
	}
	
	c->setName( n );
	g.addChild( c );
}

static GraphComponentPtr parent( GraphComponent &g )
{
	return g.parent<GraphComponent>();
}

struct UnarySlotCaller
{
	boost::signals::detail::unusable operator()( boost::python::object slot, GraphComponentPtr g )
	{
		try
		{
			slot( g );
		}
		catch( const error_already_set &e )
		{
			PyErr_Print(); // clears the error status
		}
		return boost::signals::detail::unusable();
	}
};

struct BinarySlotCaller
{

	boost::signals::detail::unusable operator()( boost::python::object slot, GraphComponentPtr g, GraphComponentPtr gg )
	{
		try
		{
			slot( g, gg );
		}
		catch( const error_already_set &e )
		{
			PyErr_Print(); // clears the error status
		}
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
		.def( "__getitem__", (GraphComponentPtr (GraphComponent::*)( const std::string & ))&GraphComponent::getChild<GraphComponent> )
		.def( "__setitem__", setItem )
		.def( "children", &children )
		.def( "parent", &parent )
		.def( "ancestor", (GraphComponentPtr (GraphComponent::*)( IECore::TypeId ))&GraphComponent::ancestor )
		.def( "commonAncestor", (GraphComponentPtr (GraphComponent::*)( ConstGraphComponentPtr, IECore::TypeId ))&GraphComponent::commonAncestor )
		.def( "childAddedSignal", &GraphComponent::childAddedSignal, return_internal_reference<1>() )
		.def( "childRemovedSignal", &GraphComponent::childRemovedSignal, return_internal_reference<1>() )
		.def( "parentChangedSignal", &GraphComponent::parentChangedSignal, return_internal_reference<1>() )
		.IE_COREPYTHON_DEFRUNTIMETYPEDSTATICMETHODS( GraphComponent )
	;
	
	SignalBinder<GraphComponent::UnarySignal, DefaultSignalCaller<GraphComponent::UnarySignal>, UnarySlotCaller>::bind( "UnarySignal" );
	SignalBinder<GraphComponent::BinarySignal, DefaultSignalCaller<GraphComponent::BinarySignal>, BinarySlotCaller>::bind( "BinarySignal" );
		
	INTRUSIVE_PTR_PATCH( GraphComponent, GraphComponentPyClass );
	
	implicitly_convertible<GraphComponentPtr, IECore::RunTimeTypedPtr>();
	implicitly_convertible<GraphComponentPtr, ConstGraphComponentPtr>();

}
