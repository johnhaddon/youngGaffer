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
	if( plug->getFlags( Plug::Dynamic ) )
	{
		// we need to serialise the whole plug
		std::string plugSerialisation = s.serialiseC( plug );
		return "\"" + plug->relativeName( ancestor ) + "\" : " + plugSerialisation + ", ";	
	}
	else
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
	}
	return "";
}

std::string serialiseNode( Serialiser &s, ConstGraphComponentPtr g )
{
	ConstNodePtr node = boost::static_pointer_cast<const Node>( g );
	
	std::string result = boost::str( boost::format( "%s.%s( \"%s\", " )
		% s.modulePath( g )
		% node->typeName()
		% node->getName()
	);

	result += "**{ ";
	for( InputPlugIterator pIt=node->inputPlugsBegin(); pIt!=pIt.end(); pIt++ )
	{
		PlugPtr plug = *pIt;
		std::string ps = serialisePlug( s, node, *pIt );
		result += ps;
	}
	result += " }";
	result += ")";
	return result;
}

class NodeWrapper : public Node, public IECore::Wrapper<Node>
{

	public :
		
		NodeWrapper( PyObject *self, const std::string &name=staticTypeName() )
			:	Node( name ), IECore::Wrapper<Node>( self, this )
		{
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

void GafferBindings::setPlugs( NodePtr node, const boost::python::dict &keywords )
{
	list items = keywords.items();
	long l = len( items );
	for( long i=0; i<l; i++ )
	{
		std::string name = extract<std::string>( items[i][0] );

		PlugPtr plug = node->getChild<Plug>( name );
		if( !plug )
		{
			// plug doesn't exist yet, hopefully a dynamic plug is being added
			extract<PlugPtr> plugExtractor( items[i][1] );
			if( plugExtractor.check() )
			{
				// add dynamic plug
				PlugPtr p = plugExtractor();
				node->addChild( p );
			}
			else
			{
				std::string err = boost::str( boost::format( "No plug named \"%s\"." ) % name );
				throw std::invalid_argument( err.c_str() );	
			}
		}
		else
		{
			// plug already exists, connect it or set its value
			setPlugValue( plug, items[i][1] );
		}
	}
}

// really we want a void return type but raw_function doesn't seem to like that
static bool setPlugsRaw( tuple t, dict d )
{
	if( len( t ) > 1 )
	{
		throw std::invalid_argument( "Expected only keyword arguments." );
	}
	NodePtr node = extract<NodePtr>( t[0] );
	setPlugs( node, d );
	return true;
}

static NodePtr constructor( tuple t, dict d )
{
	long l = len( t );
	if( !l )
	{
		throw std::invalid_argument( "Expected self for first argument to constructor." );
	}
	if( l>2 )
	{
		throw std::invalid_argument( "Too many arguments." );
	}
	
	std::string name = Node::staticTypeName();
	if( l==2 )
	{
		name = extract<std::string>( t[1] )();
	}
		
	NodePtr result = new NodeWrapper( ((object)t[0]).ptr(), name );
	setPlugs( result, d );
	return result;
}

void GafferBindings::bindNode()
{
	
	scope s = IECore::RunTimeTypedClass<Node>()
		.def( "__init__", rawConstructor( constructor ) )
		.def( "scriptNode", (ScriptNodePtr (Node::*)())&Node::scriptNode )
		.def( "setPlugs", raw_function( setPlugsRaw, 1 ) )
		.def( "plugSetSignal", &Node::plugSetSignal, return_internal_reference<1>() )
		.def( "plugDirtiedSignal", &Node::plugDirtiedSignal, return_internal_reference<1>() )
		.def( "plugInputChangedSignal", &Node::plugInputChangedSignal, return_internal_reference<1>() )
	;
	
	SignalBinder<Node::UnaryPlugSignal, DefaultSignalCaller<Node::UnaryPlugSignal>, CatchingSlotCaller<Node::UnaryPlugSignal> >::bind( "UnaryPlugSignal" );
	SignalBinder<Node::BinaryPlugSignal, DefaultSignalCaller<Node::BinaryPlugSignal>, CatchingSlotCaller<Node::BinaryPlugSignal> >::bind( "BinaryPlugSignal" );
	
	Serialiser::registerSerialiser( Node::staticTypeId(), serialiseNode );
		
}
