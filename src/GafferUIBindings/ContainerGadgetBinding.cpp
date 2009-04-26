#include "boost/python.hpp"

#include "GafferUIBindings/ContainerGadgetBinding.h"
#include "GafferUI/ContainerGadget.h"

#include "IECore/bindings/RunTimeTypedBinding.h"

using namespace boost::python;
using namespace GafferUIBindings;
using namespace GafferUI;

void GafferUIBindings::bindContainerGadget()
{
	IECore::RunTimeTypedClass<ContainerGadget>()
	;
}
