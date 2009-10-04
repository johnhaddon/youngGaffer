#ifndef GAFFERBINDINGS_CATCHINGSLOTCALLER_INL
#define GAFFERBINDINGS_CATCHINGSLOTCALLER_INL

namespace GafferBindings
{

template<int Arity, typename Signal>
struct CatchingSlotCallerBase;

template<typename Signal>
struct CatchingSlotCallerBase<1, Signal>
{
	typename Signal::slot_result_type operator()( boost::python::object slot, typename Signal::arg1_type a1 )
	{
		try
		{
			return boost::python::extract<typename Signal::slot_result_type>( slot( a1 ) )();
		}
		catch( const boost::python::error_already_set &e )
		{
			PyErr_PrintEx( 0 ); // also clears the python error status
			return typename Signal::slot_result_type();
		}
	}
};

template<typename Signal>
struct CatchingSlotCallerBase<2, Signal>
{
	typename Signal::slot_result_type operator()( boost::python::object slot, typename Signal::arg1_type a1, typename Signal::arg2_type a2 )
	{
		try
		{
			return boost::python::extract<typename Signal::slot_result_type>( slot( a1, a2 ) )();
		}
		catch( const boost::python::error_already_set &e )
		{
			PyErr_PrintEx( 0 ); // also clears the python error status
			return typename Signal::slot_result_type();
		}
	}
};

template<typename Signal>
struct CatchingSlotCallerBase<3, Signal>
{
	typename Signal::slot_result_type operator()( boost::python::object slot, typename Signal::arg1_type a1, typename Signal::arg2_type a2, typename Signal::arg3_type a3 )
	{
		try
		{
			return boost::python::extract<typename Signal::slot_result_type>( slot( a1, a2, a3 ) )();
		}
		catch( const boost::python::error_already_set &e )
		{
			PyErr_PrintEx( 0 ); // also clears the python error status
			return typename Signal::slot_result_type();
		}
	}
};

template<typename Signal>
struct CatchingSlotCaller : public CatchingSlotCallerBase<Signal::slot_function_type::arity, Signal>
{
};


} // namespace GafferBindings

#include "GafferBindings/CatchingSlotCaller.inl"

#endif GAFFERBINDINGS_CATCHINGSLOTCALLER_INL

