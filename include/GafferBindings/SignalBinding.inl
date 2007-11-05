#ifndef GAFFERBINDINGS_SIGNALBINDING_INL
#define GAFFERBINDINGS_SIGNALBINDING_INL

#include "GafferBindings/ConnectionBinding.h"

namespace GafferBindings
{

struct SignalBindings
{
	typedef boost::signal<int ( float )> Signal;
	
	static int call( Signal &s, float f )
	{
		return s( f );
	}

};

template<typename Signal>
void bindSignal( const char *className )
{

	boost::python::class_<Signal, boost::noncopyable>( className )
		.def( "connect", &Connection::create<Signal> )
		.def( "__call__", &SignalBindings::call )
	;

}

} // namespace GafferBindings

#endif // GAFFERBINDINGS_SIGNALBINDING_INL
