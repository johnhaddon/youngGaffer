#include "boost/python.hpp"

#include "GafferUIBindings/GadgetBinding.h"
#include "GafferUI/Gadget.h"
#include "GafferUI/Style.h"

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
		.def( "getStyle", &Gadget::getStyle )
		.def( "setStyle", &Gadget::setStyle )
		.def( "getTransform", &Gadget::getTransform, return_value_policy<copy_const_reference>() )
		.def( "setTransform", &Gadget::setTransform )
		.def( "fullTransform", (Imath::M44f (Gadget::*)())&Gadget::fullTransform )
		.def( "fullTransform", &Gadget::fullTransform )
		.def( "render",&Gadget::render )
		.def( "bound", &Gadget::bound )
		.def( "transformedBound", (Imath::Box3f (Gadget::*)() const)&Gadget::transformedBound )
		.def( "transformedBound", (Imath::Box3f (Gadget::*)( ConstGadgetPtr ) const)&Gadget::transformedBound )
		.def( "renderRequestSignal", &Gadget::renderRequestSignal, return_internal_reference<1>() )
		.def( "buttonPressSignal", &Gadget::buttonPressSignal, return_internal_reference<1>() )
		.def( "buttonReleaseSignal", &Gadget::buttonReleaseSignal, return_internal_reference<1>() )
		.def( "dragBeginSignal", &Gadget::dragBeginSignal, return_internal_reference<1>() )
		.def( "dragUpdateSignal", &Gadget::dragUpdateSignal, return_internal_reference<1>() )
		.def( "dropSignal", &Gadget::dropSignal, return_internal_reference<1>() )
		.def( "dragEndSignal", &Gadget::dragEndSignal, return_internal_reference<1>() )
		.def( "keyPressSignal", &Gadget::keyPressSignal, return_internal_reference<1>() )
		.def( "keyReleaseSignal", &Gadget::keyReleaseSignal, return_internal_reference<1>() )
		.IE_COREPYTHON_DEFRUNTIMETYPEDSTATICMETHODS( Gadget )
	;
	
	SignalBinder<Gadget::RenderRequestSignal>::bind( "RenderRequestSignal" );
	SignalBinder<Gadget::ButtonSignal>::bind( "ButtonSignal" );
	SignalBinder<Gadget::KeySignal>::bind( "KeySignal" );
	SignalBinder<Gadget::DragBeginSignal>::bind( "DragBeginSignal" );
	SignalBinder<Gadget::DragDropSignal>::bind( "DragDropSignal" );
	
	INTRUSIVE_PTR_PATCH( Gadget, GadgetPyClass );
	
	implicitly_convertible<GadgetPtr, Gaffer::GraphComponentPtr>();
	implicitly_convertible<GadgetPtr, ConstGadgetPtr>();

}
