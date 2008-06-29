#ifndef GAFFERUI_GRAPHGADGET_H
#define GAFFERUI_GRAPHGADGET_H

#include "GafferUI/ContainerGadget.h"

#include "Gaffer/Node.h"

namespace GafferUI
{

class GraphGadget : public ContainerGadget
{

	public :

		GraphGadget( Gaffer::NodePtr graphRoot );
		virtual ~GraphGadget();

		IE_CORE_DECLARERUNTIMETYPEDEXTENSION( GraphGadget, GraphGadgetTypeId, ContainerGadget );

		virtual Imath::M44f childTransform( ConstGadgetPtr child ) const;

	protected :

		bool keyPressed( GadgetPtr gadget, const KeyEvent &event );
		void childAdded( GraphComponent *parent, GraphComponent *child );
		void childRemoved( GraphComponent *parent, GraphComponent *child );
		
		bool dragBegin( GadgetPtr gadget, const ButtonEvent &event );	
		bool dragUpdate( GadgetPtr gadget, const ButtonEvent &event );
		
	private :
	
		Gaffer::Node *m_graphRoot;
	
		Imath::V2f m_lastDragPosition;

};

IE_CORE_DECLAREPTR( GraphGadget );

} // namespace GafferUI

#endif // GAFFERUI_GRAPHGADGET_H
