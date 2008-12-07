#include "boost/python.hpp"

#include "GafferBindings/ObjectSignalBinding.h"

#include "GafferBindings/SignalBinding.h"

using namespace boost::python;

void GafferBindings::bindObjectSignal()
{
	typedef boost::signal<void ( object )> ObjectSignal; 
	SignalBinder<ObjectSignal>::bind( "ObjectSignal" );
}
