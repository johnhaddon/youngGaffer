#ifndef GAFFERUI_GRAPHGADGET_H
#define GAFFERUI_GRAPHGADGET_H

#include "GafferUI/ContainerGadget.h"

#include "Gaffer/Node.h"

namespace GafferUI
{

class GraphGadget : public ContainerGadget
{

	public :

		GraphGadget( Gaffer::NodePtr parent );
		virtual ~GraphGadget();

		IE_CORE_DECLARERUNTIMETYPEDEXTENSION( GraphGadget, GraphGadgetTypeId, Gadget );

		virtual Imath::M44f childTransform( ConstGadgetPtr child ) const;

	protected :

		void childAdded( GraphComponent *parent, GraphComponent *child );
			
};

IE_CORE_DECLAREPTR( GraphGadget );

} // namespace GafferUI

#endif // GAFFERUI_GRAPHGADGET_H
