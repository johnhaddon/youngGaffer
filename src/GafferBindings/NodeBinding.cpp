#include "boost/python.hpp"
#include "boost/python/raw_function.hpp"

#include "boost/format.hpp"

#include "GafferBindings/NodeBinding.h"
#include "GafferBindings/ValuePlugBinding.h"
#include "GafferBindings/SignalBinding.h"
#include "GafferBindings/RawConstructor.h"
#include "GafferBindings/CatchingSlotCaller.h"
#include "GafferBindings/Serialiser.h"
#include "Gaffer/ScriptNode.h"
#include "Gaffer/CompoundPlug.h"

#include "IECore/bindings/Wrapper.h"
#include "IECore/bindings/RunTimeTypedBinding.h"

using namespace boost::python;
using namespace GafferBindings;
using namespace Gaffer;

static std::string serialisePlug( Serialiser &s, ConstGraphComponentPtr ancestor, PlugPtr plug )
{
	// not dynamic, we can just serialise the connection/value				
	if( plug->isInstanceOf( CompoundPlug::staticTypeId() ) )
	{
		std::string result;
		CompoundPlug *cPlug = static_cast<CompoundPlug *>( plug.get() );
		InputPlugIterator pIt( cPlug->children().begin(), cPlug->children().end() );
		while( pIt!=cPlug->children().end() )
		{
			result += serialisePlug( s, ancestor, *pIt++ );
		}
		return result;
	}

	std::string value = serialisePlugValue( s, plug );
	if( value!="" )
	{
		return "\"" + plug->relativeName( ancestor ) + "\" : " + value + ", ";		
	}

	return "";
}

static std::string serialiseNode( Serialiser &s, ConstGraphComponentPtr g )
{
	ConstNodePtr node = boost::static_pointer_cast<const Node>( g );
	
	std::string result = boost::str( boost::format( "%s.%s( \"%s\", " )
		% s.modulePath( g )
		% node->typeName()
		% node->getName()
	);

	// non dynamic input plugs
	std::string inputs = "";
	for( InputPlugIterator pIt=node->inputPlugsBegin(); pIt!=pIt.end(); pIt++ )
	{
		PlugPtr plug = *pIt;
		if( !plug->getFlags( Plug::Dynamic ) )
		{
			inputs += serialisePlug( s, node, *pIt );
		}
	}
	
	if( inputs.size() )
	{
		result += "inputs = { " + inputs + "}, ";
	}

	// dynamic plugs of any direction
	std::string dynamicPlugs = "";
	for( PlugIterator pIt=node->plugsBegin(); pIt!=pIt.end(); pIt++ )
	{
		PlugPtr plug = *pIt;
		if( plug->getFlags( Plug::Dynamic ) )
		{
			dynamicPlugs += s.serialiseC( plug ) + ", ";
		}	
	}

	if( dynamicPlugs.size() )
	{
		result += "dynamicPlugs = ( " + dynamicPlugs + "), ";
	}
	
	result += ")";
	return result;
}

class NodeWrapper : public Node, public IECore::Wrapper<Node>
{

	public :
		
		NodeWrapper( PyObject *self, const std::string &name=staticTypeName(), const dict &inputs, const tuple &dynamicPlugs )
			:	Node( name ), IECore::Wrapper<Node>( self, this )
		{
			initNode( this, inputs, dynamicPlugs );
		}		
		
		virtual void dirty( ConstPlugPtr dirty ) const
		{
			if( override f = this->get_override( "dirty" ) )
			{
				f( boost::const_pointer_cast<Plug>( dirty ) );
			}
		}
		
		virtual void compute( PlugPtr output ) const
		{
			if( override f = this->get_override( "compute" ) )
			{
				f( boost::const_pointer_cast<Plug>( output ) );
			}
		}
		
		IE_COREPYTHON_RUNTIMETYPEDWRAPPERFNS( Node );

};

IE_CORE_DECLAREPTR( NodeWrapper );

static void setPlugs( Node *node, const boost::python::dict &inputs )
{
	list items = inputs.items();
	long l = len( items );
	for( long i=0; i<l; i++ )
	{
		std::string name = extract<std::string>( items[i][0] );

		PlugPtr plug = node->getChild<Plug>( name );
		if( !plug )
		{
			std::string err = boost::str( boost::format( "No plug named \"%s\"." ) % name );
			throw std::invalid_argument( err.c_str() );	
		}
		else
		{
			setPlugValue( plug, items[i][1] );
		}
	}
}

static void addDynamicPlugs( Node *node, const boost::python::tuple &dynamicPlugs )
{
	long l = len( dynamicPlugs );
	for( long i=0; i<l; i++ )
	{
		PlugPtr p = extract<PlugPtr>( dynamicPlugs[i] );
		node->addChild( p );
	}
}

void GafferBindings::initNode( Node *node, const boost::python::dict &inputs, const boost::python::tuple &dynamicPlugs )
{
	setPlugs( node, inputs );
	addDynamicPlugs( node, dynamicPlugs );
}

void GafferBindings::bindNode()
{
	
	scope s = IECore::RunTimeTypedClass<Node, NodeWrapperPtr>()
		.def( 	init< const std::string &, const dict &, const tuple & >
				(
					(
						arg( "name" ) = Node::staticTypeName(),
						arg( "inputs" ) = dict(),
						arg( "dynamicPlugs" ) = tuple()
					)
				)
		)
		.def( "scriptNode", (ScriptNodePtr (Node::*)())&Node::scriptNode )
		.def( "_init", &initNode )
		.def( "plugSetSignal", &Node::plugSetSignal, return_internal_reference<1>() )
		.def( "plugDirtiedSignal", &Node::plugDirtiedSignal, return_internal_reference<1>() )
		.def( "plugInputChangedSignal", &Node::plugInputChangedSignal, return_internal_reference<1>() )
	;
	
	SignalBinder<Node::UnaryPlugSignal, DefaultSignalCaller<Node::UnaryPlugSignal>, CatchingSlotCaller<Node::UnaryPlugSignal> >::bind( "UnaryPlugSignal" );
	SignalBinder<Node::BinaryPlugSignal, DefaultSignalCaller<Node::BinaryPlugSignal>, CatchingSlotCaller<Node::BinaryPlugSignal> >::bind( "BinaryPlugSignal" );
	
	Serialiser::registerSerialiser( Node::staticTypeId(), serialiseNode );
		
}
