#include "boost/python.hpp"

#include "GafferUIBindings/GadgetBinding.h"
#include "GafferUI/Gadget.h"
#include "GafferUI/Style.h"

#include "GafferBindings/SignalBinding.h"
#include "GafferBindings/CatchingSlotCaller.h"

#include "IECorePython/RunTimeTypedBinding.h"

using namespace boost::python;
using namespace GafferUIBindings;
using namespace GafferBindings;
using namespace GafferUI;

struct RenderRequestSlotCaller
{
	boost::signals::detail::unusable operator()( boost::python::object slot, GadgetPtr g )
	{
		slot( g );
		return boost::signals::detail::unusable();
	}
};

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS( fullTransformOverloads, fullTransform, 0, 1 );

void GafferUIBindings::bindGadget()
{
	scope s = IECorePython::RunTimeTypedClass<Gadget>()
		.def( "getStyle", &Gadget::getStyle )
		.def( "setStyle", &Gadget::setStyle )
		.def( "getTransform", &Gadget::getTransform, return_value_policy<copy_const_reference>() )
		.def( "setTransform", &Gadget::setTransform )
		.def( "fullTransform", &Gadget::fullTransform, fullTransformOverloads() )
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
	;
	
	SignalBinder<Gadget::RenderRequestSignal, DefaultSignalCaller<Gadget::RenderRequestSignal>, RenderRequestSlotCaller>::bind( "RenderRequestSignal" );	
	SignalBinder<Gadget::ButtonSignal, DefaultSignalCaller<Gadget::ButtonSignal>, CatchingSlotCaller<Gadget::ButtonSignal> >::bind( "ButtonSignal" );
	SignalBinder<Gadget::KeySignal, DefaultSignalCaller<Gadget::KeySignal>, CatchingSlotCaller<Gadget::KeySignal> >::bind( "KeySignal" );
	SignalBinder<Gadget::DragBeginSignal, DefaultSignalCaller<Gadget::DragBeginSignal>, CatchingSlotCaller<Gadget::DragBeginSignal> >::bind( "DragBeginSignal" );
	SignalBinder<Gadget::DragDropSignal, DefaultSignalCaller<Gadget::DragDropSignal>, CatchingSlotCaller<Gadget::DragDropSignal> >::bind( "DragDropSignal" );

}
