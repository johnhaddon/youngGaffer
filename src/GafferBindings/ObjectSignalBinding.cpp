#include "boost/python.hpp"

#include "GafferBindings/ObjectSignalBinding.h"
#include "GafferBindings/SignalBinding.h"
#include "GafferBindings/CatchingSlotCaller.h"

using namespace boost::python;

void GafferBindings::bindObjectSignal()
{
	typedef boost::signal<void ( object )> ObjectSignal; 
	SignalBinder<ObjectSignal, DefaultSignalCaller<ObjectSignal>, CatchingSlotCaller<ObjectSignal> >::bind( "ObjectSignal" );
}
