#include "boost/python.hpp"

#include "GafferUIBindings/ButtonEventBinding.h"
#include "GafferUI/ButtonEvent.h"

using namespace boost::python;
using namespace GafferUIBindings;
using namespace GafferUI;

void GafferUIBindings::bindButtonEvent()
{
	scope s = class_<ButtonEvent, bases<ModifiableEvent> >( "ButtonEvent" )
		.def( init<ButtonEvent::Buttons>() )
		.def( init<ButtonEvent::Buttons, const IECore::LineSegment3f &>() )
		.def( init<ButtonEvent::Buttons, const IECore::LineSegment3f &, ModifiableEvent::Modifiers>() )
		.def_readwrite( "buttons", &ButtonEvent::buttons )
		.def_readwrite( "line", &ButtonEvent::line )
	;
	
	enum_<ButtonEvent::Buttons>( "Buttons" )
		.value( "None", ButtonEvent::None )
		.value( "Left", ButtonEvent::Left )
		.value( "Middle", ButtonEvent::Middle )
		.value( "Right", ButtonEvent::Right )
		.value( "LeftMiddle", ButtonEvent::LeftMiddle )
		.value( "RightMiddle", ButtonEvent::RightMiddle )
		.value( "LeftRight", ButtonEvent::LeftRight )
		.value( "All", ButtonEvent::All )
	;
}
