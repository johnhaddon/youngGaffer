#include "boost/python.hpp"

#include "GafferUIBindings/ModifiableEventBinding.h"
#include "GafferUI/ModifiableEvent.h"

using namespace boost::python;
using namespace GafferUIBindings;
using namespace GafferUI;

void GafferUIBindings::bindModifiableEvent()
{
	scope s = class_<ModifiableEvent, bases<Event> >( "ModifiableEvent" )
		.def( init<ModifiableEvent::Modifiers>() )
		.def_readwrite( "modifiers", &ModifiableEvent::modifiers )
	;
	
	enum_<ModifiableEvent::Modifiers>( "Modifiers" )
		.value( "None", ModifiableEvent::None )
		.value( "Shift", ModifiableEvent::Shift )
		.value( "Control", ModifiableEvent::Control )
		.value( "Alt", ModifiableEvent::Alt )
		.value( "ShiftControl", ModifiableEvent::ShiftControl )
		.value( "ShiftAlt", ModifiableEvent::ShiftAlt )
		.value( "ControlAlt", ModifiableEvent::ControlAlt )
		.value( "All", ModifiableEvent::All )
	;
}
