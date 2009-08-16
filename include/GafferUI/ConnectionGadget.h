#ifndef GAFFERUI_CONNECTIONGADGET_H
#define GAFFERUI_CONNECTIONGADGET_H

#include "GafferUI/Gadget.h"

#include "Gaffer/Plug.h"

namespace GafferUI
{
	IE_CORE_FORWARDDECLARE( Nodule )
}

namespace GafferUI
{

class ConnectionGadget : public Gadget
{

	public :

		ConnectionGadget( GafferUI::NodulePtr srcNodule, GafferUI::NodulePtr dstNodule );
		virtual ~ConnectionGadget();

		IE_CORE_DECLARERUNTIMETYPEDEXTENSION( ConnectionGadget, ConnectionGadgetTypeId, Gadget );
		
		/// Accepts only GraphGadgets as parent.
		virtual bool acceptsParent( const Gaffer::GraphComponent *potentialParent ) const;		
		virtual Imath::Box3f bound() const;
		
		NodulePtr srcNodule();
		NodulePtr dstNodule();
		
	protected :

		void setSrcPos( const Imath::V3f &p );
		void setDstPos( const Imath::V3f &p );
		void setPositionsFromNodules();
		
		void doRender( IECore::RendererPtr renderer ) const;

	private :
		
		bool buttonPress( GadgetPtr gadget, const ButtonEvent &event );
		IECore::RunTimeTypedPtr dragBegin( GadgetPtr gadget, const DragDropEvent &event );	
		bool dragUpdate( GadgetPtr gadget, const DragDropEvent &event );
		bool dragEnd( GadgetPtr gadget, const DragDropEvent &event );
		
		Imath::V3f m_srcPos;
		Imath::V3f m_dstPos;
		
		NodulePtr m_srcNodule;
		NodulePtr m_dstNodule;
		
		Gaffer::Plug::Direction m_dragEnd;
		
};

IE_CORE_DECLAREPTR( ConnectionGadget );

} // namespace GafferUI

#endif // GAFFERUI_CONNECTIONGADGET_H
