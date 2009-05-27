#include "boost/python.hpp"

#include "GafferBindings/TypedObjectPlugBinding.h"
#include "Gaffer/TypedObjectPlug.h"
#include "Gaffer/Node.h"

#include "IECore/bindings/Wrapper.h"
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
static void bind()
{
	typedef typename T::ValuePtr V;
	
	IECore::RunTimeTypedClass<T>()
		.def( init<const std::string &, Plug::Direction, V, unsigned>(
				(
					boost::python::arg_( "name" )=T::staticTypeName(),
					boost::python::arg_( "direction" )=Plug::In,
					boost::python::arg_( "defaultValue" )=V(),
					boost::python::arg_( "flags" )=Plug::None
				)
			)
		)
		.def( "defaultValue", &T::defaultValue )
		.def( "setValue", &T::setValue )
		.def( "getValue", getValue<T>, "Returns a copy of the value." )
	;

}

void GafferBindings::bindTypedObjectPlug()
{
	bind<ObjectPlug>();
}
