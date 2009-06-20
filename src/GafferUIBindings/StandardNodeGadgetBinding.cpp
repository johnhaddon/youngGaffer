#include "boost/python.hpp"

#include "GafferUIBindings/StandardNodeGadgetBinding.h"
#include "GafferUI/StandardNodeGadget.h"

#include "IECore/bindings/RunTimeTypedBinding.h"

using namespace boost::python;
using namespace GafferUIBindings;
using namespace GafferUI;

void GafferUIBindings::bindStandardNodeGadget()
{
	IECore::RunTimeTypedClass<StandardNodeGadget>()
		.def( init<Gaffer::NodePtr>() )
	;
}
