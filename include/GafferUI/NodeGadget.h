#ifndef GAFFERUI_NODEGADGET_H
#define GAFFERUI_NODEGADGET_H

#include "GafferUI/Gadget.h"

#include "Gaffer/Node.h"

namespace GafferUI
{

class NodeGadget : public Gadget
{

	public :

		NodeGadget( Gaffer::NodePtr node );
		virtual ~NodeGadget();

		Gaffer::NodePtr node();
		Gaffer::ConstNodePtr node() const;

		IE_CORE_DECLARERUNTIMETYPEDEXTENSION( NodeGadget, NodeGadgetTypeId, Gadget );

		virtual Imath::Box3f bound() const;

	protected :
	
		virtual void doRender( IECore::RendererPtr renderer ) const;

	private :
		
		Gaffer::Node *m_node;
		
};

IE_CORE_DECLAREPTR( NodeGadget );

} // namespace GafferUI

#endif // GAFFERUI_NODEGADGET_H
