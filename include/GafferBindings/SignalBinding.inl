#ifndef GAFFERBINDINGS_SIGNALBINDING_INL
#define GAFFERBINDINGS_SIGNALBINDING_INL

#include "GafferBindings/ConnectionBinding.h"

namespace GafferBindings
{

template<int Arity, typename Signal>
struct DefaultSignalCallerBase;

template<typename Signal>
struct DefaultSignalCallerBase<1, Signal>
{
	static typename Signal::result_type call( Signal &s, typename Signal::arg2_type a2 )
	{
		return s( a2 );
	}
};

template<typename Signal>
struct DefaultSignalCallerBase<2, Signal>
{
	static typename Signal::result_type call( Signal &s, typename Signal::arg2_type a2, typename Signal::arg3_type a3 )
	{
		return s( a2, a3 );
	}
};

template<typename Signal>
struct DefaultSignalCaller : public DefaultSignalCallerBase<Signal::slot_function_type::arity, Signal>
{

};

template<typename Signal, typename SignalCaller, typename SlotCaller>
void SignalBinder<Signal, SignalCaller, SlotCaller>::bind( const char *className )
{

	boost::python::class_<Signal, boost::noncopyable>( className )
		.def( "connect", &Connection::create<Signal, SlotCaller> )
		.def( "__call__", &SignalCaller::call )
	;

}

} // namespace GafferBindings

#endif // GAFFERBINDINGS_SIGNALBINDING_INL
