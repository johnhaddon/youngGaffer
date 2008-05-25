#include "boost/python.hpp"

#include "GafferUIBindings/IndividualContainerBinding.h"
#include "GafferUI/IndividualContainer.h"

#include "IECore/bindings/IntrusivePtrPatch.h"
#include "IECore/bindings/RunTimeTypedBinding.h"

using namespace boost::python;
using namespace GafferUIBindings;
using namespace GafferUI;

void GafferUIBindings::bindIndividualContainer()
{
	typedef class_<IndividualContainer, IndividualContainerPtr, boost::noncopyable, bases<ContainerGadget> > IndividualContainerPyClass;

	IndividualContainerPyClass( "IndividualContainer", init<GadgetPtr>() )
		.def( "setChild", &IndividualContainer::setChild )
		.def( "getChild", (GadgetPtr (IndividualContainer::*)())&IndividualContainer::getChild<Gadget> )
		// we also have to redefine the GraphComponent level getChild binding so that we don't override it completely
		.def( "getChild", (Gaffer::GraphComponentPtr (Gaffer::GraphComponent::*)( const std::string & ))&Gaffer::GraphComponent::getChild<Gaffer::GraphComponent> )
		.IE_COREPYTHON_DEFRUNTIMETYPEDSTATICMETHODS( IndividualContainer )
	;
		
	INTRUSIVE_PTR_PATCH( IndividualContainer, IndividualContainerPyClass );
	
	implicitly_convertible<IndividualContainerPtr, ContainerGadgetPtr>();
	implicitly_convertible<IndividualContainerPtr, ConstIndividualContainerPtr>();

}
