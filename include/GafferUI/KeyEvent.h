#ifndef GAFFERUI_KEYEVENT_H
#define GAFFERUI_KEYEVENT_H

#include "GafferUI/ModifiableEvent.h"

#include "OpenEXR/ImathVec.h"

namespace GafferUI
{

/// A class to represent events involving keyboard keys.
struct KeyEvent : public ModifiableEvent
{
	KeyEvent(
		char k = 'a',
		Modifiers m = ModifiableEvent::None
	)
		:	ModifiableEvent( m ), key( k )
	{
	};
	
	/// The key pressed.
	/// \todo Might be a better type to use than char?
	char key;
		
};

} // namespace GafferUI

#endif // GAFFERUI_KEYEVENT_H
