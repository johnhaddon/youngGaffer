#include "boost/python.hpp"

#include "GafferBindings/TypedObjectPlugBinding.h"
#include "GafferBindings/Serialiser.h"
#include "GafferBindings/PlugBinding.h"
#include "Gaffer/TypedObjectPlug.h"
#include "Gaffer/Node.h"

#include "IECorePython/Wrapper.h"
#include "IECorePython/RunTimeTypedBinding.h"

using namespace boost::python;
using namespace GafferBindings;
using namespace Gaffer;

/// \todo Should we be able to serialise values and default values?
template<typename T>
static std::string serialise( Serialiser &s, ConstGraphComponentPtr g )
{
	typename T::ConstPtr plug = IECore::staticPointerCast<const T>( g );
	std::string result = s.modulePath( g ) + "." + g->typeName() + "( \"" + g->getName() + "\", ";
	
	if( plug->direction()!=Plug::In )
	{
		result += "direction = " + serialisePlugDirection( plug->direction() ) + ", ";
	}
		
	if( plug->getFlags() )
	{
		result += "flags = " + serialisePlugFlags( plug->getFlags() ) + ", ";
	}
	
	bool connected = false;
	ConstPlugPtr srcPlug = plug->template getInput<Plug>();
	if( srcPlug )
	{
		std::string srcNodeName = s.add( srcPlug->node() );
		if( srcNodeName!="" )
		{
			connected = true;
			result += "input = " + srcNodeName + "[\"" + srcPlug->getName() + "\"]";
		}
	}
	
	result += ")";

	return result;
}

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
static typename T::Ptr construct(
	const char *name,
	Plug::Direction direction,
	typename T::ValuePtr defaultValue,
	unsigned flags,
	PlugPtr input
)
{
	typename T::Ptr result = new T( name, direction, defaultValue, flags );
	if( input )
	{
		result->setInput( input );
	}
	return result;
}

template<typename T>
static void bind()
{
	typedef typename T::ValuePtr V;
	
	IECorePython::RunTimeTypedClass<T>()
		.def( "__init__", make_constructor( construct<T>, default_call_policies(), 
				(
					boost::python::arg_( "name" )=T::staticTypeName(),
					boost::python::arg_( "direction" )=Plug::In,
					boost::python::arg_( "defaultValue" )=typename T::ValuePtr(),
					boost::python::arg_( "flags" )=Plug::None,
					boost::python::arg_( "input" )=PlugPtr( 0 )
				)
			)
		)
		.def( "defaultValue", &T::defaultValue )
		.def( "setValue", &T::setValue )
		.def( "getValue", getValue<T>, "Returns a copy of the value." )
	;

	Serialiser::registerSerialiser( T::staticTypeId(), serialise<T> );

}

void GafferBindings::bindTypedObjectPlug()
{
	bind<ObjectPlug>();
}
