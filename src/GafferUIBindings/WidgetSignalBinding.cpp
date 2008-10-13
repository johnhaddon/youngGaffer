#include "boost/python.hpp"

#include "GafferUIBindings/WidgetSignalBinding.h"

#include "GafferUI/EventSignalCombiner.h"

#include "GafferBindings/SignalBinding.h"

using namespace boost::python;
using namespace GafferBindings;
using namespace GafferUI;

void GafferUIBindings::bindWidgetSignal()
{
	typedef boost::signal<bool ( object ), EventSignalCombiner<bool> > WidgetSignal; 
	SignalBinder<WidgetSignal>::bind( "WidgetSignal" );

}
