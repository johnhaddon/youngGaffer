#include "boost/python.hpp"

#include "GafferUIBindings/DragDropEventBinding.h"
#include "GafferUI/DragDropEvent.h"

using namespace boost::python;
using namespace GafferUIBindings;
using namespace GafferUI;

void GafferUIBindings::bindDragDropEvent()
{
	class_<DragDropEvent, bases<ButtonEvent> >( "DragDropEvent" )
		.def( init<ButtonEvent::Buttons>() )
		.def( init<ButtonEvent::Buttons, const IECore::LineSegment3f &>() )
		.def( init<ButtonEvent::Buttons, const IECore::LineSegment3f &, ModifiableEvent::Modifiers>() )
		.def_readwrite( "data", &DragDropEvent::data )
	;	
}
