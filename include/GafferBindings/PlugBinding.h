#ifndef GAFFERBINDINGS_PLUGBINDING_H
#define GAFFERBINDINGS_PLUGBINDING_H

#include "GafferBindings/Serialiser.h"
#include "Gaffer/Plug.h"

namespace GafferBindings
{

void bindPlug();

std::string serialisePlugDirection( Gaffer::Plug::Direction direction );
std::string serialisePlugFlags( unsigned flags );
std::string serialisePlugInput( Serialiser &s, Gaffer::PlugPtr plug );

} // namespace GafferBindings

#endif // GAFFERBINDINGS_PLUGBINDING_H
