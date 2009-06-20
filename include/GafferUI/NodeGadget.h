#ifndef GAFFERUI_NODEGADGET_H
#define GAFFERUI_NODEGADGET_H

#include "GafferUI/IndividualContainer.h"

#include "Gaffer/Set.h"

namespace GafferUI
{

IE_CORE_FORWARDDECLARE( Nodule )
IE_CORE_FORWARDDECLARE( NodeGadget )

/// A base class for representing nodes within a GraphGadget.
class NodeGadget : public IndividualContainer
{

	public :

		IE_CORE_DECLARERUNTIMETYPEDEXTENSION( NodeGadget, NodeGadgetTypeId, ContainerGadget );
		
		Gaffer::NodePtr node();
		Gaffer::ConstNodePtr node() const;

		/// Should be overridden by derived classes to return a nodule for
		/// the plug if it has one, and 0 otherwise.
		virtual NodulePtr nodule( Gaffer::ConstPlugPtr plug );
		virtual ConstNodulePtr nodule( Gaffer::ConstPlugPtr plug ) const;
				
		/// Creates a NodeGadget for the specified node.
		static NodeGadgetPtr create( Gaffer::NodePtr node );
		
		typedef boost::function<NodeGadgetPtr ( Gaffer::NodePtr )> NodeGadgetCreator;
		/// Registers a function which will return a NodeGadget instance for a node of a specific
		/// type. This can be used to customise the NodeGadget for specific node types.
		static void registerNodeGadget( IECore::TypeId nodeType, NodeGadgetCreator creator );
		
	protected :

		NodeGadget( Gaffer::NodePtr node );
		virtual ~NodeGadget();
	
		/// Creating a static one of these is a convenient way of registering a NodeGadget type.
		template<class T>
		struct NodeGadgetTypeDescription
		{
			NodeGadgetTypeDescription( IECore::TypeId nodeType ) { NodeGadget::registerNodeGadget( nodeType, &creator ); };
			static NodeGadgetPtr creator( Gaffer::NodePtr node ) { return new T( node ); };
		};
		
	private :
				
		Gaffer::Node *m_node;
		
		typedef std::map<IECore::TypeId, NodeGadgetCreator> CreatorMap;
		static CreatorMap g_creators;
		
};

IE_CORE_DECLAREPTR( NodeGadget );

} // namespace GafferUI

#endif // GAFFERUI_NODEGADGET_H
