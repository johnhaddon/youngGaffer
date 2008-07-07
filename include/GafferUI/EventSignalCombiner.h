#ifndef GAFFERUI_EVENTSIGNALCOMBINER_H
#define GAFFERUI_EVENTSIGNALCOMBINER_H

namespace GafferUI
{

/// The EventSignalCombiner is used in the definition of the various Gadget signals. It
/// calls each slot in order until one returns true, at which point it shortcuts and returns
/// without calling the other slots.
template<typename T>
struct EventSignalCombiner
{

	typedef T result_type;

	template<typename InputIterator>
	result_type operator()( InputIterator first, InputIterator last ) const;
 
};

} // namespace GafferUI

#include "GafferUI/EventSignalCombiner.inl"

#endif // GAFFERUI_EVENTSIGNALCOMBINER_H
