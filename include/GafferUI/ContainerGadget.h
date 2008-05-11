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

		/// Must be implemented by derived classes to say where the child belongs within
		/// the container.
		virtual Imath::M44f childTransform( ConstGadgetPtr child ) const = 0;
		
	protected :
	
		/// Implemented to render each child with it's appropriate transform.
		virtual void doRender( IECore::RendererPtr renderer ) const;
		
	private :
	
		static void childAdded( GraphComponent *us, GraphComponent *child );
		static void childRemoved( GraphComponent *us, GraphComponent *child );
		static void childRenderRequest( GadgetPtr child );
		
};

IE_CORE_DECLAREPTR( ContainerGadget );

} // namespace GafferUI

#endif // GAFFERUI_CONTAINERGADGET_H
