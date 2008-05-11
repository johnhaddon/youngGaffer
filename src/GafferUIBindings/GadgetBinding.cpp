#include "boost/python.hpp"

#include "GafferUIBindings/GadgetBinding.h"
#include "GafferUI/Gadget.h"

#include "GafferBindings/SignalBinding.h"

#include "IECore/bindings/IntrusivePtrPatch.h"
#include "IECore/bindings/RunTimeTypedBinding.h"

using namespace boost::python;
using namespace GafferUIBindings;
using namespace GafferBindings;
using namespace GafferUI;

void GafferUIBindings::bindGadget()
{
	typedef class_<Gadget, GadgetPtr, boost::noncopyable, bases<Gaffer::GraphComponent> > GadgetPyClass;

	scope s = GadgetPyClass( "Gadget", no_init )
		.def( "render",&Gadget::render )
		.def( "bound", &Gadget::bound )
		.def( "renderRequestSignal", &Gadget::renderRequestSignal, return_internal_reference<1>() )
		.def( "buttonPressSignal", &Gadget::buttonPressSignal, return_internal_reference<1>() )
		.def( "buttonReleaseSignal", &Gadget::buttonReleaseSignal, return_internal_reference<1>() )
		.def( "keyPressSignal", &Gadget::keyPressSignal, return_internal_reference<1>() )
		.def( "keyReleaseSignal", &Gadget::keyReleaseSignal, return_internal_reference<1>() )
		.IE_COREPYTHON_DEFRUNTIMETYPEDSTATICMETHODS( Gadget )
	;
	
	SignalBinder<Gadget::RenderRequestSignal>::bind( "RenderRequestSignal" );
	SignalBinder<Gadget::ButtonSignal>::bind( "ButtonSignal" );
	SignalBinder<Gadget::KeySignal>::bind( "KeySignal" );
	
	INTRUSIVE_PTR_PATCH( Gadget, GadgetPyClass );
	
	implicitly_convertible<GadgetPtr, IECore::RunTimeTypedPtr>();
	implicitly_convertible<GadgetPtr, ConstGadgetPtr>();

}
