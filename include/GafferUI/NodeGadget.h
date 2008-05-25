#ifndef GAFFERUI_NODEGADGET_H
#define GAFFERUI_NODEGADGET_H

#include "GafferUI/Frame.h"

#include "Gaffer/Node.h"

namespace GafferUI
{

class NodeGadget : public Frame
{

	public :

		NodeGadget( Gaffer::NodePtr node );
		virtual ~NodeGadget();

		IE_CORE_DECLARERUNTIMETYPEDEXTENSION( NodeGadget, NodeGadgetTypeId, Frame );
		
		Gaffer::NodePtr node();
		Gaffer::ConstNodePtr node() const;

		/// Accepts no children.
		virtual bool acceptsChild( Gaffer::ConstGraphComponentPtr potentialChild ) const;

	private :
		
		Gaffer::Node *m_node;
		
};

IE_CORE_DECLAREPTR( NodeGadget );

} // namespace GafferUI

#endif // GAFFERUI_NODEGADGET_H
