#include "boost/python.hpp"

#include "GafferUIBindings/RenderableGadgetBinding.h"
#include "GafferUI/RenderableGadget.h"

#include "IECorePython/RunTimeTypedBinding.h"

using namespace boost::python;
using namespace GafferUIBindings;
using namespace GafferUI;

void GafferUIBindings::bindRenderableGadget()
{
	IECorePython::RunTimeTypedClass<RenderableGadget>()
		.def( init<IECore::VisibleRenderablePtr>() )
		.def( "setRenderable", &RenderableGadget::setRenderable )
		.def( "getRenderable", (IECore::VisibleRenderablePtr (RenderableGadget::*)())&RenderableGadget::getRenderable )
	;

}
