#ifndef GAFFERBINDINGS_RAWCONSTRUCTOR_H
#define GAFFERBINDINGS_RAWCONSTRUCTOR_H

#include "boost/functional.hpp"

namespace GafferBindings
{

template<typename F>
boost::python::object rawConstructor( F f );

} // namespace GafferBindings

#include "GafferBindings/RawConstructor.inl"

#endif GAFFERBINDINGS_RAWCONSTRUCTOR_H
