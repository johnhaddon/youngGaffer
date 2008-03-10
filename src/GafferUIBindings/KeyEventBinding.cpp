#include "boost/python.hpp"

#include "GafferUIBindings/KeyEventBinding.h"
#include "GafferUI/KeyEvent.h"

using namespace boost::python;
using namespace GafferUIBindings;
using namespace GafferUI;

void GafferUIBindings::bindKeyEvent()
{
	class_<KeyEvent, bases<ModifiableEvent> >( "KeyEvent" )
		.def( init<char>() )
		.def( init<char, ModifiableEvent::Modifiers>() )
		.def_readwrite( "key", &KeyEvent::key )
	;
}
