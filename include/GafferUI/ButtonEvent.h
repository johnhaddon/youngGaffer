#ifndef GAFFERUI_BUTTONEVENT_H
#define GAFFERUI_BUTTONEVENT_H

#include "GafferUI/ModifiableEvent.h"

#include "OpenEXR/ImathVec.h"

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
		const Imath::V3f &rayOrigin = Imath::V3f( 0.0f ),
		const Imath::V3f &rayDirection = Imath::V3f( 0.0f ),
		Modifiers m = ModifiableEvent::None
	)
		:	ModifiableEvent( m ), buttons( b ), origin( rayOrigin ), direction( rayDirection )
	{
	};
	
	Buttons buttons;
	Imath::V3f origin;
	Imath::V3f direction;
	
};

} // namespace GafferUI

#endif // GAFFERUI_BUTTONEVENT_H
