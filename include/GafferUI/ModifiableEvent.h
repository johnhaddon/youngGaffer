#ifndef GAFFERUI_MODIFIABLEEVENT_H
#define GAFFERUI_MODIFIABLEEVENT_H

#include "GafferUI/Event.h"

namespace GafferUI
{

/// A base class for events for which the keyboard
/// modifier keys are relevant.
struct ModifiableEvent : public Event
{
	/// An enum to represent the modifier keys. We deliberately ignore
	/// platform specific modifiers like the Apple and Windows keys to
	/// provide a consistent set of shortcuts across platforms.
	enum Modifiers
	{
		None = 0,
		Shift = 1,
		Control = 2,
		Alt = 3,
		ShiftControl = Shift | Control,
		ShiftAlt = Shift | Alt,
		ControlAlt = Control | Alt,
		All = Shift | Control | Alt
	};
	
	ModifiableEvent( Modifiers m = None ) : modifiers( m ) {};

	/// The state of the modifier keys.
	Modifiers modifiers;
};

} // namespace GafferUI

#endif // GAFFERUI_MODIFIABLEEVENT_H
