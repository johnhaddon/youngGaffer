#include "boost/python.hpp"

#include "GafferBindings/NumericPlugBinding.h"
#include "Gaffer/NumericPlug.h"
#include "Gaffer/Node.h"

#include "IECore/bindings/Wrapper.h"
#include "IECore/bindings/RunTimeTypedBinding.h"

using namespace boost::python;
using namespace GafferBindings;
using namespace Gaffer;

template<typename T>
static void bind( const char *name )
{
	typedef typename T::ValueType V;
	
	IECore::RunTimeTypedClass<T>()
		.def( init<const std::string &, Plug::Direction, V, V, V, unsigned, PlugPtr>(
				(
					boost::python::arg_( "name" )=T::staticTypeName(),
					boost::python::arg_( "direction" )=Plug::In,
					boost::python::arg_( "defaultValue" )=V(),
					boost::python::arg_( "minValue" )=Imath::limits<V>::min(),
					boost::python::arg_( "maxValue" )=Imath::limits<V>::max(),
					boost::python::arg_( "flags" )=Plug::None,
					boost::python::arg_( "input" )=PlugPtr( 0 )
				)
			)
		)
		.def( "defaultValue", &T::defaultValue )
		.def( "minValue", &T::minValue )
		.def( "maxValue", &T::maxValue )
		.def( "setValue", &T::setValue )
		.def( "getValue", &T::getValue )
	;

}

void GafferBindings::bindNumericPlug()
{
	bind<FloatPlug>( "FloatPlug" );
	bind<IntPlug>( "IntPlug" );
}
