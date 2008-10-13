#ifndef GAFFERUI_CONTAINERGADGET_H
#define GAFFERUI_CONTAINERGADGET_H

#include "GafferUI/Gadget.h"

namespace GafferUI
{

class ContainerGadget : public Gadget
{

	public :

		ContainerGadget( const std::string &name=staticTypeName() );
		virtual ~ContainerGadget();

		IE_CORE_DECLARERUNTIMETYPEDEXTENSION( ContainerGadget, ContainerGadgetTypeId, Gadget );

		/// ContainerGadgets accept any number of other Gadgets as children. Derived classes
		/// may further restrict this if they wish, but they must not accept non-Gadget children.
		virtual bool acceptsChild( Gaffer::ConstGraphComponentPtr potentialChild ) const;
		/// Returns the union of the transformed bounding boxes of all children.
		virtual Imath::Box3f bound() const;
		//@}
		
	protected :
	
		/// Implemented to render all the children.
		virtual void doRender( IECore::RendererPtr renderer ) const;
		
	private :
	
		void childAdded( GraphComponent *parent, GraphComponent *child );
		void childRemoved( GraphComponent *parent, GraphComponent *child );
		void childRenderRequest( Gadget *child );
		
};

IE_CORE_DECLAREPTR( ContainerGadget );

} // namespace GafferUI

#endif // GAFFERUI_CONTAINERGADGET_H
