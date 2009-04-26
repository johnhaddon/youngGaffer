#ifndef GAFFERBINDINGS_SETBINDING_H
#define GAFFERBINDINGS_SETBINDING_H

namespace GafferBindings
{

/// Binds the standard set types.
void bindSet();

/// Can be used to bind custom set types.
template <typename T>
void bindSet();

} // namespace GafferBindings

#include "GafferBindings/SetBinding.inl"

#endif GAFFERBINDINGS_SETBINDING_H
