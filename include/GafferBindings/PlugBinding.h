#ifndef GAFFERBINDINGS_PLUGBINDING_H
#define GAFFERBINDINGS_PLUGBINDING_H

#include "Gaffer/Plug.h"

namespace GafferBindings
{

void bindPlug();

std::string serialisePlugDirection( Gaffer::Plug::Direction direction );
std::string serialisePlugFlags( unsigned flags );

} // namespace GafferBindings

#endif // GAFFERBINDINGS_PLUGBINDING_H
