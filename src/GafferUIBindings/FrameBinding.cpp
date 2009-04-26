#include "boost/python.hpp"

#include "GafferUIBindings/FrameBinding.h"
#include "GafferUI/Frame.h"

#include "IECore/bindings/RunTimeTypedBinding.h"

using namespace boost::python;
using namespace GafferUIBindings;
using namespace GafferUI;

void GafferUIBindings::bindFrame()
{
	IECore::RunTimeTypedClass<Frame>()
		.def( init<GadgetPtr>() )
	;
}
