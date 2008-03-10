#include "boost/python.hpp"

#include "GafferUIBindings/EventBinding.h"
#include "GafferUI/Event.h"

using namespace boost::python;
using namespace GafferUIBindings;
using namespace GafferUI;

void GafferUIBindings::bindEvent()
{
	class_<Event>( "Event" )
	;
}
