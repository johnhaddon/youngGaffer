#include "boost/python.hpp"

#include "GafferUIBindings/RenderableGadgetBinding.h"
#include "GafferUI/RenderableGadget.h"

#include "IECore/bindings/IntrusivePtrPatch.h"
#include "IECore/bindings/RunTimeTypedBinding.h"

using namespace boost::python;
using namespace GafferUIBindings;
using namespace GafferUI;

void GafferUIBindings::bindRenderableGadget()
{
	typedef class_<RenderableGadget, RenderableGadgetPtr, boost::noncopyable, bases<Gadget> > RenderableGadgetPyClass;

	RenderableGadgetPyClass( "RenderableGadget", init<IECore::VisibleRenderablePtr>() )
		.def( "setRenderable", &RenderableGadget::setRenderable )
		.def( "getRenderable", (IECore::VisibleRenderablePtr (RenderableGadget::*)())&RenderableGadget::getRenderable )
		.IE_COREPYTHON_DEFRUNTIMETYPEDSTATICMETHODS( RenderableGadget )
	;
	
	INTRUSIVE_PTR_PATCH( RenderableGadget, RenderableGadgetPyClass );
	
	implicitly_convertible<RenderableGadgetPtr, GadgetPtr>();
	implicitly_convertible<RenderableGadgetPtr, ConstRenderableGadgetPtr>();

}
