#ifndef GAFFERUI_GRAPHGADGET_H
#define GAFFERUI_GRAPHGADGET_H

#include "GafferUI/ContainerGadget.h"

namespace Gaffer
{
IE_CORE_FORWARDDECLARE( Node );
IE_CORE_FORWARDDECLARE( Plug );
IE_CORE_FORWARDDECLARE( ScriptNode );
}

namespace GafferUI
{

IE_CORE_FORWARDDECLARE( NodeGadget );
IE_CORE_FORWARDDECLARE( ConnectionGadget );

class GraphGadget : public ContainerGadget
{

	public :

		GraphGadget( Gaffer::NodePtr graphRoot );
		virtual ~GraphGadget();

		IE_CORE_DECLARERUNTIMETYPEDEXTENSION( GraphGadget, GraphGadgetTypeId, ContainerGadget );

	protected :

		void childAdded( GraphComponent *parent, GraphComponent *child );
		void childRemoved( GraphComponent *parent, GraphComponent *child );
		void inputChanged( Gaffer::PlugPtr dstPlug );
		void plugSet( Gaffer::PlugPtr plug );
	
		bool keyPressed( GadgetPtr gadget, const KeyEvent &event );
		
		bool buttonPress( GadgetPtr gadget, const ButtonEvent &event );
		bool buttonRelease( GadgetPtr gadget, const ButtonEvent &event );
		
		IECore::RunTimeTypedPtr dragBegin( GadgetPtr gadget, const ButtonEvent &event );	
		bool dragUpdate( GadgetPtr gadget, const ButtonEvent &event );
		
		void addNodeGadget( Gaffer::Node *node );
		NodeGadget *nodeGadget( Gaffer::Node *node );
		void updateNodeGadgetTransform( NodeGadget *nodeGadget );
		
		void addConnectionGadget( Gaffer::Plug *dstPlug );
		ConnectionGadget *connectionGadget( Gaffer::Plug *dstPlug );
		
	private :
	
		Gaffer::ScriptNodePtr script();
	
		Gaffer::Node *m_graphRoot;
		
		typedef std::map<Gaffer::Node *, NodeGadget *> NodeGadgetMap;
		NodeGadgetMap m_nodeGadgets;
	
		typedef std::map<const Gaffer::Plug *, ConnectionGadget *> ConnectionGadgetMap;
		ConnectionGadgetMap m_connectionGadgets;

		Imath::V2f m_lastDragPosition;
		

};

IE_CORE_DECLAREPTR( GraphGadget );

} // namespace GafferUI

#endif // GAFFERUI_GRAPHGADGET_H
