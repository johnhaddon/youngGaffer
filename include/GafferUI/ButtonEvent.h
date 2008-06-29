#ifndef GAFFERUI_BUTTONEVENT_H
#define GAFFERUI_BUTTONEVENT_H

#include "GafferUI/ModifiableEvent.h"

#include "IECore/LineSegment.h"

namespace GafferUI
{

/// A class to represent events involving mouse buttons.
struct ButtonEvent : public ModifiableEvent
{
	/// An enum to represent the mouse buttons.
	enum Buttons
	{
		None = 0,
		Left = 1,
		Middle = 2,
		Right = 4,
		LeftMiddle = Left | Middle,
		RightMiddle = Right | Middle,
		LeftRight = Left | Right,
		All = Left | Middle | Right
	};

	ButtonEvent(
		Buttons b=None,
		const IECore::LineSegment3f &Line=IECore::LineSegment3f(),
		Modifiers m = ModifiableEvent::None
	)
		:	ModifiableEvent( m ), buttons( b ), line( Line )
	{
	};
	
	Buttons buttons;
	IECore::LineSegment3f line;
	
};

} // namespace GafferUI

#endif // GAFFERUI_BUTTONEVENT_H
