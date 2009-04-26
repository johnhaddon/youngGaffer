#include "boost/python.hpp"

#include "GafferUIBindings/ConnectionGadgetBinding.h"
#include "GafferUI/ConnectionGadget.h"
#include "GafferUI/Nodule.h"

#include "Gaffer/Node.h"

#include "IECore/bindings/RunTimeTypedBinding.h"

using namespace boost::python;
using namespace GafferUIBindings;
using namespace GafferUI;

void GafferUIBindings::bindConnectionGadget()
{
	IECore::RunTimeTypedClass<ConnectionGadget>()
		.def( init<GafferUI::NodulePtr, GafferUI::NodulePtr>() )
	;
}
