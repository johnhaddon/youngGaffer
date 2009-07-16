#ifndef GAFFERBINDINGS_VALUEPLUGBINDING_H
#define GAFFERBINDINGS_VALUEPLUGBINDING_H

#include "boost/python.hpp"

#include "GafferBindings/Serialiser.h"
#include "Gaffer/ValuePlug.h"

namespace GafferBindings
{

/// \todo I wonder if this should go in Serialiser anyway, and have a
/// registration mechanism?
std::string serialisePlugValue( Serialiser &s, Gaffer::PlugPtr plug );

void setPlugValue( Gaffer::PlugPtr plug, boost::python::object value );

void bindValuePlug();

} // namespace GafferBindings

#endif // GAFFERBINDINGS_VALUEPLUGBINDING_H
