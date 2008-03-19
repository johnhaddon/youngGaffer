#ifndef GAFFERUI_RENDERABLEGADGET_H
#define GAFFERUI_RENDERABLEGADGET_H

#include "GafferUI/Gadget.h"

#include "IECore/VisibleRenderable.h"

namespace GafferUI
{

IE_CORE_FORWARDDECLARE( RenderableGadget );

class RenderableGadget : public Gadget
{

	public :

		RenderableGadget( IECore::VisibleRenderablePtr renderable );
		virtual ~RenderableGadget();

		IE_CORE_DECLARERUNTIMETYPEDEXTENSION( RenderableGadget, RenderableGadgetTypeId, Gadget );

		virtual Imath::Box3f bound( IECore::RendererPtr renderer ) const;
		
		void setRenderable( IECore::VisibleRenderablePtr renderable );
		IECore::VisibleRenderablePtr getRenderable();
		IECore::ConstVisibleRenderablePtr getRenderable() const;
		
	protected :
	
		virtual void doRender( IECore::RendererPtr renderer ) const;
		
	private :
	
		IECore::VisibleRenderablePtr m_renderable;
};

} // namespace GafferUI

#endif // GAFFERUI_RenderableGadget_H
