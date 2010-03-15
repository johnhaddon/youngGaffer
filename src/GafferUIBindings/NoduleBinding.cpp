#include "boost/python.hpp"

#include "GafferUIBindings/NoduleBinding.h"
#include "GafferUI/Nodule.h"

#include "Gaffer/Plug.h"

#include "IECorePython/RunTimeTypedBinding.h"

using namespace boost::python;
using namespace GafferUIBindings;
using namespace GafferUI;

void GafferUIBindings::bindNodule()
{
	IECorePython::RunTimeTypedClass<Nodule>()
		.def( init<Gaffer::PlugPtr>() )
	;
}
