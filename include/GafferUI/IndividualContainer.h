#ifndef GAFFERUI_INDIVIDUALCONTAINER_H
#define GAFFERUI_INDIVIDUALCONTAINER_H

#include "GafferUI/ContainerGadget.h"

namespace GafferUI
{

/// The IndividualContainer class allows a single child to be held,
/// and rejects efforts to add any more.
class IndividualContainer : public ContainerGadget
{

	public :

		IndividualContainer( GadgetPtr child );
		virtual ~IndividualContainer();

		IE_CORE_DECLARERUNTIMETYPEDEXTENSION( IndividualContainer, IndividualContainerTypeId, ContainerGadget );

		/// Accepts the child only if there are currently no children.
		virtual bool acceptsChild( Gaffer::ConstGraphComponentPtr potentialChild ) const;
		
		/// Returns the identity matrix - derived classes may override this if need be.
		virtual Imath::M44f childTransform( ConstGadgetPtr child ) const;
		
		/// Removes the current child if there is one, and replaces it
		/// with the specified gadget.
		void setChild( GadgetPtr child );
		/// Returns the child, performing a runTimeCast to T.
		template<typename T>
		typename T::Ptr getChild();
		/// Returns the child, performing a runTimeCast to T.
		template<typename T>
		typename T::ConstPtr getChild() const;
		
						
};

IE_CORE_DECLAREPTR( IndividualContainer );

} // namespace GafferUI

#include "GafferUI/IndividualContainer.inl"

#endif // GAFFERUI_INDIVIDUALCONTAINER_H
