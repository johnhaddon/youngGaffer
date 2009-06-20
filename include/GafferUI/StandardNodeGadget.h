#ifndef GAFFERUI_STANDARDNODEGADGET_H
#define GAFFERUI_STANDARDNODEGADGET_H

#include "GafferUI/NodeGadget.h"

namespace GafferUI
{

IE_CORE_FORWARDDECLARE( LinearContainer )

class StandardNodeGadget : public NodeGadget
{

	public :
	
		IE_CORE_DECLARERUNTIMETYPEDEXTENSION( StandardNodeGadget, StandardNodeGadgetTypeId, NodeGadget );

		StandardNodeGadget( Gaffer::NodePtr node );
		virtual ~StandardNodeGadget();

		virtual NodulePtr nodule( Gaffer::ConstPlugPtr plug );
		virtual ConstNodulePtr nodule( Gaffer::ConstPlugPtr plug ) const;
		
		Imath::Box3f bound() const;

	protected :
	
		virtual void doRender( IECore::RendererPtr renderer ) const;

	private :
	
		NodulePtr addNodule( Gaffer::PlugPtr plug );
	
		static NodeGadgetTypeDescription<StandardNodeGadget> g_nodeGadgetTypeDescription;
		
		bool m_nodeHasObjectPlugs;
		
		typedef std::map<const Gaffer::Plug *, Nodule *> NoduleMap;
		NoduleMap m_nodules;
				
		void selectionChanged( Gaffer::NodeSetPtr selection, Gaffer::NodePtr node );
		void childAdded( Gaffer::GraphComponentPtr parent, Gaffer::GraphComponentPtr child );
		void childRemoved( Gaffer::GraphComponentPtr parent, Gaffer::GraphComponentPtr child );

};

} // namespace GafferUI

#endif // GAFFERUI_STANDARDNODEGADGET_H
