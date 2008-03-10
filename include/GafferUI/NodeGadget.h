#ifndef GAFFERUI_NODEGADGET_H
#define GAFFERUI_NODEGADGET_H

#include "GafferUI/Gadget.h"

namespace GafferUI
{

class NodeGadget : public Gadget
{

	public :

		NodeGadget();
		virtual ~NodeGadget();

		IE_CORE_DECLARERUNTIMETYPEDEXTENSION( NodeGadget, NodeGadgetTypeId, Gadget );

		virtual Imath::Box3f bound( IECore::RendererPtr renderer ) const;

	protected :
	
		virtual void doRender( IECore::RendererPtr renderer ) const;
		
};

IE_CORE_DECLAREPTR( NodeGadget );

} // namespace GafferUI

#endif // GAFFERUI_NODEGADGET_H
