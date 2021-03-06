#ifndef GAFFERUI_LINEARCONTAINER_H
#define GAFFERUI_LINEARCONTAINER_H

#include "GafferUI/ContainerGadget.h"

namespace GafferUI
{

class LinearContainer : public ContainerGadget
{

	public :

		enum Orientation
		{
			InvalidOrientation,
			X,
			Y,
			Z
		};
		
		enum Alignment
		{
			InvalidAlignment,
			Min,
			Centre,
			Max
		};
		
		LinearContainer( const std::string &name=staticTypeName(), Orientation orientation=X,
			Alignment alignment=Centre, float spacing = 0.0f );
			
		virtual ~LinearContainer();

		IE_CORE_DECLARERUNTIMETYPEDEXTENSION( LinearContainer, LinearContainerTypeId, ContainerGadget );

		void setOrientation( Orientation orientation );
		Orientation getOrientation() const;

		void setAlignment( Alignment alignment );
		Alignment getAlignment() const;
		
		void setSpacing( float spacing );
		float getSpacing() const;

		virtual Imath::Box3f bound() const;

	protected :

		virtual void doRender( IECore::RendererPtr renderer ) const;
				
	private :
	
		void renderRequested( GadgetPtr gadget );
	
		Orientation m_orientation;
		Alignment m_alignment;
		float m_spacing;
		
		mutable bool m_clean;
		void calculateChildTransforms() const;
		
};

IE_CORE_DECLAREPTR( LinearContainer );

} // namespace GafferUI

#endif // GAFFERUI_LINEARCONTAINER_H
