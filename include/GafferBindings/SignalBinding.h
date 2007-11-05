#ifndef GAFFERBINDINGS_SIGNALBINDING_H
#define GAFFERBINDINGS_SIGNALBINDING_H

namespace GafferBindings
{

/// Binds the boost::signal type T under the given
/// class name.
template<typename Signal>
void bindSignal( const char *className );

} // namespace GafferBindings

#include "GafferBindings/SignalBinding.inl"

#endif // GAFFERBINDINGS_SIGNALBINDING_H
