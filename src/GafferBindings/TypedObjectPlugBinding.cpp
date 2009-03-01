#include "boost/python.hpp"

#include "GafferBindings/TypedObjectPlugBinding.h"
#include "Gaffer/TypedObjectPlug.h"
#include "Gaffer/Node.h"

#include "IECore/bindings/Wrapper.h"
#include "IECore/bindings/WrapperToPython.h"
#include "IECore/bindings/IntrusivePtrPatch.h"
#include "IECore/bindings/RunTimeTypedBinding.h"

using namespace boost::python;
using namespace GafferBindings;
using namespace Gaffer;

template<typename T>
static typename T::ValuePtr getValue( typename T::Ptr p )
{
	typename T::ConstValuePtr v = p->getValue();
	if( v )
	{
		return v->copy();
	}
	return 0;
}

template<typename T>
static void bind( const char *name )
{
	typedef class_<T, typename T::Ptr, boost::noncopyable, bases<ValuePlug> > TypedObjectPlugPyClass;
	typedef typename T::ValuePtr V;
	
	TypedObjectPlugPyClass( name, no_init )
		.def( init<const std::string &, Plug::Direction, V, unsigned, PlugPtr>(
				(
					boost::python::arg_( "name" )=T::staticTypeName(),
					boost::python::arg_( "direction" )=Plug::In,
					boost::python::arg_( "defaultValue" )=V(),
					boost::python::arg_( "flags" )=Plug::None,
					boost::python::arg_( "input" )=PlugPtr( 0 )
				)
			)
		)
		.def( "defaultValue", &T::defaultValue )
		.def( "setValue", &T::setValue )
		.def( "getValue", getValue<T>, "Returns a copy of the value." )
		.IE_COREPYTHON_DEFRUNTIMETYPEDSTATICMETHODS( T )
	;

	INTRUSIVE_PTR_PATCH( T, typename TypedObjectPlugPyClass );
	
	implicitly_convertible<typename T::Ptr, ValuePlugPtr>();
	implicitly_convertible<typename T::Ptr, typename T::ConstPtr>();

}

void GafferBindings::bindTypedObjectPlug()
{
	bind<ObjectPlug>( "ObjectPlug" );
}
