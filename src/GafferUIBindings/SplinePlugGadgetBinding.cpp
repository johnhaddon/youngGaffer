#include "boost/python.hpp"

#include "GafferUIBindings/SplinePlugGadgetBinding.h"
#include "GafferUI/SplinePlugGadget.h"

#include "IECorePython/RunTimeTypedBinding.h"

using namespace boost::python;
using namespace GafferUIBindings;
using namespace GafferUI;

void GafferUIBindings::bindSplinePlugGadget()
{
	IECorePython::RunTimeTypedClass<SplinePlugGadget>()
		.def( init<>() )
		.def( "splines", (Gaffer::SetPtr (SplinePlugGadget::*)())&SplinePlugGadget::splines )
		.def( "selection", (Gaffer::SetPtr (SplinePlugGadget::*)())&SplinePlugGadget::selection )
	;
}
