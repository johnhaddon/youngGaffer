#ifndef GAFFERUI_GRAPHGADGET_H
#define GAFFERUI_GRAPHGADGET_H

#include "GafferUI/ContainerGadget.h"

namespace Gaffer
{
IE_CORE_FORWARDDECLARE( Node );
IE_CORE_FORWARDDECLARE( Plug );
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

		bool keyPressed( GadgetPtr gadget, const KeyEvent &event );
		void childAdded( GraphComponent *parent, GraphComponent *child );
		void childRemoved( GraphComponent *parent, GraphComponent *child );
		void inputChanged( Gaffer::PlugPtr dstPlug );
		
		IECore::RunTimeTypedPtr dragBegin( GadgetPtr gadget, const ButtonEvent &event );	
		bool dragUpdate( GadgetPtr gadget, const ButtonEvent &event );
		
		NodeGadget *nodeGadget( Gaffer::Node *node );
		ConnectionGadget *connectionGadget( Gaffer::Plug *plug );
		
	private :
	
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
