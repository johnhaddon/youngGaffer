#include "boost/python.hpp"

#include "GafferUIBindings/RenderableGadgetBinding.h"
#include "GafferUI/RenderableGadget.h"

#include "IECore/bindings/RunTimeTypedBinding.h"

using namespace boost::python;
using namespace GafferUIBindings;
using namespace GafferUI;

void GafferUIBindings::bindRenderableGadget()
{
	IECore::RunTimeTypedClass<RenderableGadget>()
		.def( init<IECore::VisibleRenderablePtr>() )
		.def( "setRenderable", &RenderableGadget::setRenderable )
		.def( "getRenderable", (IECore::VisibleRenderablePtr (RenderableGadget::*)())&RenderableGadget::getRenderable )
	;

}
