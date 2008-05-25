#include "boost/python.hpp"

#include "GafferUIBindings/FrameBinding.h"
#include "GafferUI/Frame.h"

#include "IECore/bindings/IntrusivePtrPatch.h"
#include "IECore/bindings/RunTimeTypedBinding.h"

using namespace boost::python;
using namespace GafferUIBindings;
using namespace GafferUI;

void GafferUIBindings::bindFrame()
{
	typedef class_<Frame, FramePtr, boost::noncopyable, bases<IndividualContainer> > FramePyClass;

	FramePyClass( "Frame", init<GadgetPtr>() )
		.IE_COREPYTHON_DEFRUNTIMETYPEDSTATICMETHODS( Frame )
	;
		
	INTRUSIVE_PTR_PATCH( Frame, FramePyClass );
	
	implicitly_convertible<FramePtr, IndividualContainerPtr>();
	implicitly_convertible<FramePtr, ConstFramePtr>();

}
