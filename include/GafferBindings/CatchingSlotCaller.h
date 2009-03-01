#ifndef GAFFERBINDINGS_CATCHINGSLOTCALLER_H
#define GAFFERBINDINGS_CATCHINGSLOTCALLER_H

namespace GafferBindings
{

/// This struct provides an alternative SlotCaller for use with
/// SignalBinding. It ensures that any python execution errors
/// within the slot are not propagated up to the caller, and
/// therefore don't block the continued execution of other slots.
/// If an execution error does occur in the slot then the CatchingSlotCaller
/// will return a default constructed result.
template<typename Signal>
struct CatchingSlotCaller;

} // namespace GafferBindings

#include "GafferBindings/CatchingSlotCaller.inl"

#endif GAFFERBINDINGS_CATCHINGSLOTCALLER_H
