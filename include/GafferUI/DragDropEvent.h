#ifndef GAFFERUI_DRAGDROPEVENT_H
#define GAFFERUI_DRAGDROPEVENT_H

#include "GafferUI/ButtonEvent.h"

#include "IECore/RunTimeTyped.h"

namespace GafferUI
{

struct DragDropEvent : public ButtonEvent
{

	DragDropEvent(
		Buttons b=None,
		const IECore::LineSegment3f &Line=IECore::LineSegment3f(),
		Modifiers m = ModifiableEvent::None
	)
		:	ButtonEvent( b, Line, m ), data( 0 )
	{
	};
	
	/// An object representing the data being dragged.
	IECore::RunTimeTypedPtr data;

};

} // namespace GafferUI

#endif GAFFERUI_DRAGDROPEVENT_H
