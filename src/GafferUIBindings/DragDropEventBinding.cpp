#include "boost/python.hpp"

#include "GafferUIBindings/DragDropEventBinding.h"
#include "GafferUI/DragDropEvent.h"
#include "GafferUI/Gadget.h"

using namespace boost::python;
using namespace GafferUIBindings;
using namespace GafferUI;

static GadgetPtr getSource( DragDropEvent &d )
{
	return d.source;
}

static void setSource( DragDropEvent &d, GadgetPtr s )
{
	d.source = s;
}

static IECore::RunTimeTypedPtr getData( DragDropEvent &d )
{
	return d.data;
}

static void setData( DragDropEvent &d, IECore::RunTimeTypedPtr s )
{
	d.data = s;
}

static GadgetPtr getDestination( DragDropEvent &d )
{
	return d.destination;
}

static void setDestination( DragDropEvent &d, GadgetPtr s )
{
	d.destination = s;
}

void GafferUIBindings::bindDragDropEvent()
{
	class_<DragDropEvent, bases<ButtonEvent> >( "DragDropEvent" )
		.def( init<ButtonEvent::Buttons>() )
		.def( init<ButtonEvent::Buttons, const IECore::LineSegment3f &>() )
		.def( init<ButtonEvent::Buttons, const IECore::LineSegment3f &, ModifiableEvent::Modifiers>() )
		.add_property( "source", &getSource, &setSource )
		.add_property( "data", &getData, &setData )
		.add_property( "destination", &getDestination, &setDestination )
		.def_readwrite( "dropResult", &DragDropEvent::dropResult )
	;	
}
