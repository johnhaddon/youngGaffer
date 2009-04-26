#include "boost/python.hpp"

#include "GafferUIBindings/IndividualContainerBinding.h"
#include "GafferUI/IndividualContainer.h"

#include "IECore/bindings/RunTimeTypedBinding.h"

using namespace boost::python;
using namespace GafferUIBindings;
using namespace GafferUI;

void GafferUIBindings::bindIndividualContainer()
{
	IECore::RunTimeTypedClass<IndividualContainer>()
		.def( init<GadgetPtr>() )
		.def( "setChild", &IndividualContainer::setChild )
		.def( "getChild", (GadgetPtr (IndividualContainer::*)())&IndividualContainer::getChild<Gadget> )
		// we also have to redefine the GraphComponent level getChild binding so that we don't override it completely
		.def( "getChild", (Gaffer::GraphComponentPtr (Gaffer::GraphComponent::*)( const std::string & ))&Gaffer::GraphComponent::getChild<Gaffer::GraphComponent> )
	;
}
