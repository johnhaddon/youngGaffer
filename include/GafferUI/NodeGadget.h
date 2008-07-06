#ifndef GAFFERUI_NODEGADGET_H
#define GAFFERUI_NODEGADGET_H

#include "GafferUI/IndividualContainer.h"

#include "Gaffer/ScriptNode.h"

namespace GafferUI
{

class NodeGadget : public IndividualContainer
{

	public :

		NodeGadget( Gaffer::NodePtr node );
		virtual ~NodeGadget();

		IE_CORE_DECLARERUNTIMETYPEDEXTENSION( NodeGadget, NodeGadgetTypeId, IndividualContainer );
		
		Gaffer::NodePtr node();
		Gaffer::ConstNodePtr node() const;

		/// Accepts no children.
		virtual bool acceptsChild( Gaffer::ConstGraphComponentPtr potentialChild ) const;

	protected :
	
		virtual void doRender( IECore::RendererPtr renderer ) const;
		
	private :
		
		Gaffer::Node *m_node;
		
		bool buttonPressed( GadgetPtr gadget, const ButtonEvent &event );
		void selectionChanged( Gaffer::NodeSetPtr selection, Gaffer::NodePtr node );
		
};

IE_CORE_DECLAREPTR( NodeGadget );

} // namespace GafferUI

#endif // GAFFERUI_NODEGADGET_H
