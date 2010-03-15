#include "boost/python.hpp"

#include "GafferUIBindings/NameGadgetBinding.h"
#include "GafferUI/NameGadget.h"

#include "IECorePython/RunTimeTypedBinding.h"

using namespace boost::python;
using namespace GafferUIBindings;
using namespace GafferUI;

void GafferUIBindings::bindNameGadget()
{
	IECorePython::RunTimeTypedClass<NameGadget>()
		.def( init<Gaffer::GraphComponentPtr>() )
	;
}
