#include "boost/python.hpp"

#include "GafferBindings/TypedPlugBinding.h"
#include "Gaffer/TypedPlug.h"
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
	typedef class_<T, typename T::Ptr, boost::noncopyable, bases<ValuePlug> > TypedPlugPyClass;
	typedef typename T::ValueType V;
	
	TypedPlugPyClass( name, no_init )
		.def( init<const std::string &, Plug::Direction, V>(
				(
					boost::python::arg_( "name" )=T::staticTypeName(),
					boost::python::arg_( "direction" )=Plug::In,
					boost::python::arg_( "defaultValue" )=V()
				)
			)
		)
		.def( "defaultValue", &T::defaultValue )
		.def( "setValue", &T::setValue )
		.def( "getValue", &T::getValue )
	;

	INTRUSIVE_PTR_PATCH( T, typename TypedPlugPyClass );
	
	implicitly_convertible<typename T::Ptr, ValuePlugPtr>();
	implicitly_convertible<typename T::Ptr, typename T::ConstPtr>();

}

void GafferBindings::bindTypedPlug()
{
	bind<StringPlug>( "StringPlug" );
}
