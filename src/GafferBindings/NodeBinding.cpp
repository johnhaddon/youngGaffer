#include "boost/python.hpp"

#include "GafferBindings/NodeBinding.h"
#include "GafferBindings/SignalBinding.h"
#include "Gaffer/Node.h"
#include "Gaffer/Plug.h"

#include "IECore/bindings/Wrapper.h"
#include "IECore/bindings/WrapperToPython.h"
#include "IECore/bindings/IntrusivePtrPatch.h"

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

void GafferBindings::bindNode()
{
	typedef class_<Node, NodeWrapperPtr, boost::noncopyable, bases<GraphComponent> > NodePyClass;

	scope s = NodePyClass( "Node" )
		.def( init<const std::string &>() )
		.def( "plugSetSignal", &Node::plugSetSignal, return_internal_reference<1>() )
		.def( "plugDirtiedSignal", &Node::plugDirtiedSignal, return_internal_reference<1>() )
		.def( "plugInputChangedSignal", &Node::plugInputChangedSignal, return_internal_reference<1>() )
	;
	
	SignalBinder<Node::UnaryPlugSignal>::bind( "UnaryPlugSignal" );
	SignalBinder<Node::BinaryPlugSignal>::bind( "BinaryPlugSignal" );
	
	IECore::WrapperToPython<NodePtr>();
	INTRUSIVE_PTR_PATCH( Node, NodePyClass );
	implicitly_convertible<NodePtr, GraphComponentPtr>();
	implicitly_convertible<NodePtr, ConstNodePtr>();
	
}
