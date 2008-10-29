#ifndef GAFFERBINDINGS_NODEBINDING_H
#define GAFFERBINDINGS_NODEBINDING_H

#include "Gaffer/Node.h"

namespace GafferBindings
{

void bindNode();

void setPlugs( Gaffer::NodePtr node, const boost::python::dict &keywords );

} // namespace GafferBindings

#endif // GAFFERBINDINGS_NODEBINDING_H
