#ifndef GAFFERUI_DRAGDROPEVENT_H
#define GAFFERUI_DRAGDROPEVENT_H

#include "GafferUI/ButtonEvent.h"

#include "IECore/RunTimeTyped.h"

namespace GafferUI
{

IE_CORE_FORWARDDECLARE( Gadget )

struct DragDropEvent : public ButtonEvent
{

	DragDropEvent(
		Buttons b=None,
		const IECore::LineSegment3f &Line=IECore::LineSegment3f(),
		Modifiers m = ModifiableEvent::None
	)
		:	ButtonEvent( b, Line, m ), source( 0 ), data( 0 ), destination( 0 ), dropResult( false )
	{
	};
	
	/// The Gadget where the drag originated.
	GafferUI::GadgetPtr source;
	/// An object representing the data being dragged.
	IECore::RunTimeTypedPtr data;
	/// The Gadget where the drag ends.
	GafferUI::GadgetPtr destination;
	/// The result returned from the drop signal handler on the destination.
	bool dropResult;
};

} // namespace GafferUI

#endif GAFFERUI_DRAGDROPEVENT_H
