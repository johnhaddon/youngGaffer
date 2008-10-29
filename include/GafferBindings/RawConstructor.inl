#ifndef GAFFERBINDINGS_RAWCONSTRUCTOR_INL
#define GAFFERBINDINGS_RAWCONSTRUCTOR_INL

#include "boost/functional.hpp"

namespace GafferBindings
{

namespace Detail
{

template <class F>
struct RawConstructorDispatcher
{

	typedef typename boost::binary_traits<F>::result_type ResultType;

	RawConstructorDispatcher( F f )
		: m_f( f )
	{
	}

	PyObject *operator()( PyObject *args, PyObject *keywords )
	{
		ResultType t = m_f(
			
			boost::python::tuple( boost::python::detail::borrowed_reference( args ) ),
			keywords ? boost::python::dict( boost::python::detail::borrowed_reference( keywords ) ) : boost::python::dict()
			
		);

		boost::python::detail::install_holder<ResultType> i( args );
		i( t );

		return boost::python::detail::none();
	}

	private:

		F m_f;

};

} // namespace Detail
  
template<typename F>
boost::python::object rawConstructor( F f )
{

	static boost::python::detail::keyword k;

	return boost::python::objects::function_object(
	
		boost::python::objects::py_function(
		
			Detail::RawConstructorDispatcher<F>( f ),
			boost::mpl::vector1<PyObject*>(),
			1,
			(std::numeric_limits<unsigned>::max)()
			
		),
		
		boost::python::detail::keyword_range( &k, &k )
	
	);
}

} // namespace GafferBindings

#endif GAFFERBINDINGS_RAWCONSTRUCTOR_INL
