#include "boost/python.hpp"

#include "GafferUIBindings/FrameBinding.h"
#include "GafferUI/Frame.h"

#include "IECorePython/RunTimeTypedBinding.h"

using namespace boost::python;
using namespace GafferUIBindings;
using namespace GafferUI;

void GafferUIBindings::bindFrame()
{
	IECorePython::RunTimeTypedClass<Frame>()
		.def( init<GadgetPtr>() )
	;
}
