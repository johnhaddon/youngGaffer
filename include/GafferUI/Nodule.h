#ifndef GAFFERUI_NODULE_H
#define GAFFERUI_NODULE_H

#include "GafferUI/Gadget.h"

namespace Gaffer
{
	IE_CORE_FORWARDDECLARE( Plug )
}

namespace GafferUI
{

class Nodule : public Gadget
{

	public :

		Nodule( Gaffer::PlugPtr plug );
		virtual ~Nodule();

		IE_CORE_DECLARERUNTIMETYPEDEXTENSION( Nodule, NoduleTypeId, Gadget );
		
		Gaffer::PlugPtr plug();
		Gaffer::ConstPlugPtr plug() const;

		virtual Imath::Box3f bound() const;

	protected :

		void doRender( IECore::RendererPtr renderer ) const;

		bool buttonPress( GadgetPtr gadget, const ButtonEvent &event );
		IECore::RunTimeTypedPtr dragBegin( GadgetPtr gadget, const ButtonEvent &event );	
		bool dragUpdate( GadgetPtr gadget, const DragDropEvent &event );
		bool dragEnd( GadgetPtr gadget, const DragDropEvent &event );

		bool drop( GadgetPtr gadget, const DragDropEvent &event );
		
	private :

		bool m_dragging;
		Imath::V3f m_dragPosition;
		
		Gaffer::PlugPtr m_plug;
		
};

IE_CORE_DECLAREPTR( Nodule );

} // namespace GafferUI

#endif // GAFFERUI_NODULE_H
