#include "boost/python.hpp"

#include "GafferBindings/NumericPlugBinding.h"
#include "Gaffer/NumericPlug.h"
#include "Gaffer/Node.h"

#include "IECore/bindings/Wrapper.h"
#include "IECore/bindings/WrapperToPython.h"
#include "IECore/bindings/IntrusivePtrPatch.h"

using namespace boost::python;
using namespace GafferBindings;
using namespace Gaffer;

template<typename T>
static void bind( const char *name )
{
	typedef class_<T, typename T::Ptr, boost::noncopyable, bases<ValuePlug> > NumericPlugPyClass;
	typedef typename T::ValueType V;
	
	NumericPlugPyClass( name, no_init )
		.def( init<Plug::Direction, V, V, V>(
				(
					boost::python::arg_( "direction" )=Plug::In,
					boost::python::arg_( "defaultValue" )=V(),
					boost::python::arg_( "minValue" )=Imath::limits<V>::min(),
					boost::python::arg_( "maxValue" )=Imath::limits<V>::max()
				)
			)
		)
		.def( "defaultValue", &T::defaultValue )
		.def( "minValue", &T::minValue )
		.def( "maxValue", &T::maxValue )
		.def( "setValue", &T::setValue )
		.def( "getValue", &T::getValue )
	;

	INTRUSIVE_PTR_PATCH( T, typename NumericPlugPyClass );
	
	implicitly_convertible<typename T::Ptr, ValuePlugPtr>();
	implicitly_convertible<typename T::Ptr, typename T::ConstPtr>();

}

void GafferBindings::bindNumericPlug()
{
	bind<FloatPlug>( "FloatPlug" );
	bind<IntPlug>( "IntPlug" );
}
