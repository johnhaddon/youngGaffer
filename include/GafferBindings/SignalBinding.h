#ifndef GAFFERBINDINGS_SIGNALBINDING_H
#define GAFFERBINDINGS_SIGNALBINDING_H

#include "GafferBindings/ConnectionBinding.h"

namespace GafferBindings
{

template<typename Signal>
struct DefaultSignalCaller;

/// This template structure is used to bind boost::signal types specified
/// by the Signal template parameter. The SignalCaller template parameter is
/// a struct type which has a static call() method which can take arguments
/// from python and call the signal. The SlotCaller template parameter is
/// a functor type which is used to call the python objects which are connected
/// to the signal as slots.
///
/// The only reason that SignalBinder is implemented as a struct
/// with a static member function, rather than just as a free function is that free functions
/// don't allow default template arguments, and default template arguments greatly simplify
/// the most common uses of this class.
template<typename Signal, typename SignalCaller=DefaultSignalCaller<Signal>, typename SlotCaller=DefaultSlotCaller<Signal> >
struct SignalBinder
{
	/// Binds the boost::signal type T under the given
	/// class name, using the SignalCaller and SlotCaller defined
	/// by the template parameters.
	static void bind( const char *className );
};

} // namespace GafferBindings

#include "GafferBindings/SignalBinding.inl"

#endif // GAFFERBINDINGS_SIGNALBINDING_H
