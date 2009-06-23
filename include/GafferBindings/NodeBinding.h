#ifndef GAFFERBINDINGS_NODEBINDING_H
#define GAFFERBINDINGS_NODEBINDING_H

#include "Gaffer/Node.h"

namespace GafferBindings
{

void bindNode();

void initNode( Gaffer::Node *node, const boost::python::dict &inputs, const boost::python::tuple &dynamicPlugs );

} // namespace GafferBindings

#endif // GAFFERBINDINGS_NODEBINDING_H
