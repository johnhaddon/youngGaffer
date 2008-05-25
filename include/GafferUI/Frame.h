#ifndef GAFFERUI_FRAME_H
#define GAFFERUI_FRAME_H

#include "GafferUI/IndividualContainer.h"

namespace GafferUI
{

/// The Frame class draws a frame around its child.
class Frame : public IndividualContainer
{

	public :

		Frame( GadgetPtr child );
		virtual ~Frame();

		IE_CORE_DECLARERUNTIMETYPEDEXTENSION( Frame, FrameTypeId, IndividualContainer );

		virtual Imath::Box3f bound() const;
	
	protected :
	
		virtual void doRender( IECore::RendererPtr renderer ) const;
	
	private :
	
		float m_border;
							
};

IE_CORE_DECLAREPTR( Frame );

} // namespace GafferUI

#endif // GAFFERUI_FRAME_H
