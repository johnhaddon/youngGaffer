#include "boost/python.hpp"
#include "boost/python/raw_function.hpp"

#include "boost/format.hpp"

#include "GafferBindings/NodeBinding.h"
#include "GafferBindings/SignalBinding.h"
#include "GafferBindings/RawConstructor.h"
#include "Gaffer/ScriptNode.h"
#include "Gaffer/Plug.h"

#include "IECore/bindings/Wrapper.h"
#include "IECore/bindings/WrapperToPython.h"
#include "IECore/bindings/IntrusivePtrPatch.h"
#include "IECore/bindings/RunTimeTypedBinding.h"

using namespace boost::python;
using namespace GafferBindings;
using namespace Gaffer;

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

};

IE_CORE_DECLAREPTR( NodeWrapper );

void GafferBindings::setPlugs( NodePtr node, const boost::python::dict &keywords )
{
	list items = keywords.items();
	long l = len( items );
	for( long i=0; i<l; i++ )
	{
		std::string name = extract<std::string>( items[i][0] );
		PlugPtr p = node->getChild<Plug>( name );
		if( !p )
		{
			std::string err = boost::str( boost::format( "No plug named \"%s\"." ) % name );
			throw std::invalid_argument( err.c_str() );
		}

		object pythonPlug( p );
		
		extract<PlugPtr> inputExtractor( items[i][1] );
		if( inputExtractor.check() )
		{
			pythonPlug.attr( "setInput" )( object( items[i][1] ) );
		}
		else
		{
			pythonPlug.attr( "setValue" )( object( items[i][1] ) );
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
	typedef class_<Node, NodeWrapperPtr, boost::noncopyable, bases<GraphComponent> > NodePyClass;

	scope s = NodePyClass( "Node", no_init )
		.def( "__init__", rawConstructor( constructor ) )
		.def( "scriptNode", (ScriptNodePtr (Node::*)())&Node::scriptNode )
		.def( "setPlugs", raw_function( setPlugsRaw, 1 ) )
		.def( "plugSetSignal", &Node::plugSetSignal, return_internal_reference<1>() )
		.def( "plugDirtiedSignal", &Node::plugDirtiedSignal, return_internal_reference<1>() )
		.def( "plugInputChangedSignal", &Node::plugInputChangedSignal, return_internal_reference<1>() )
		.IE_COREPYTHON_DEFRUNTIMETYPEDSTATICMETHODS( Node )
	;
	
	SignalBinder<Node::UnaryPlugSignal>::bind( "UnaryPlugSignal" );
	SignalBinder<Node::BinaryPlugSignal>::bind( "BinaryPlugSignal" );
	
	IECore::WrapperToPython<NodePtr>();
	INTRUSIVE_PTR_PATCH( Node, NodePyClass );
	implicitly_convertible<NodePtr, GraphComponentPtr>();
	implicitly_convertible<NodePtr, ConstNodePtr>();
	
}
