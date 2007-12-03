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
		
		NodeWrapper( PyObject *self )
			:	IECore::Wrapper<Node>( self, this )
		{
		}		
		
		virtual void dirty( ConstPlugPtr dirty ) const
		{
			if( override f = this->get_override( "dirty" ) )
			{
				f( boost::const_pointer_cast<Plug>( dirty ) );
			}
		}
		
		virtual IECore::ObjectPtr compute( ConstPlugPtr output ) const
		{
			if( override f = this->get_override( "compute" ) )
			{
				return f( boost::const_pointer_cast<Plug>( output ) );
			}
			return 0;	
		}

};

IE_CORE_DECLAREPTR( NodeWrapper );

void GafferBindings::bindNode()
{
	typedef class_<Node, NodeWrapperPtr, boost::noncopyable, bases<GraphComponent> > NodePyClass;

	scope s = NodePyClass( "Node" )
		.def( "plugSetSignal", &Node::plugSetSignal, return_internal_reference<1>() )
		.def( "plugDirtiedSignal", &Node::plugDirtiedSignal, return_internal_reference<1>() )
		.def( "plugConnectedSignal", &Node::plugConnectedSignal, return_internal_reference<1>() )
	;
	
	//bindSignal<Node::UnaryPlugSignal>( "UnaryPlugSignal" );
	//bindSignal<Node::BinaryPlugSignal>( "BinaryPlugSignal" );
	
	IECore::WrapperToPython<NodePtr>();
	INTRUSIVE_PTR_PATCH( Node, NodePyClass );
	implicitly_convertible<NodePtr, GraphComponentPtr>();
	implicitly_convertible<NodePtr, ConstNodePtr>();
	
}
