#include "boost/python.hpp"

#include "GafferBindings/TypedPlugBinding.h"
#include "GafferBindings/Serialiser.h"
#include "GafferBindings/PlugBinding.h"
#include "Gaffer/TypedPlug.h"
#include "Gaffer/Node.h"

#include "IECore/bindings/Wrapper.h"
#include "IECore/bindings/RunTimeTypedBinding.h"

using namespace boost::python;
using namespace GafferBindings;
using namespace Gaffer;

template<typename T>
static std::string serialise( Serialiser &s, ConstGraphComponentPtr g )
{
	typename T::ConstPtr plug = boost::static_pointer_cast<const T>( g );
	std::string result = s.modulePath( g ) + "." + g->typeName() + "( \"" + g->getName() + "\", ";
	
	if( plug->direction()!=Plug::In )
	{
		result += "direction = " + serialisePlugDirection( plug->direction() ) + ", ";
	}
	
	if( plug->defaultValue()!=typename T::ValueType() )
	{
		result += "defaultValue = " + boost::lexical_cast<std::string>( plug->defaultValue() ) + ", ";
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
	
	if( !connected && plug->direction()==Plug::In )
	{
		typename T::Ptr p = boost::const_pointer_cast<T>( plug );
		typename T::ValueType value = p->getValue();
		if( value!=plug->defaultValue() )
		{
			result += "value = \"" + boost::lexical_cast<std::string>( value ) + "\", ";
		}
	}
	
	result += ")";

	return result;
}

template<typename T>
static typename T::Ptr construct(
	const char *name,
	Plug::Direction direction,
	typename T::ValueType defaultValue,
	unsigned flags,
	PlugPtr input,
	object value
)
{
	typename T::Ptr result = new T( name, direction, defaultValue, flags );
	if( input && value!=object() )
	{
		throw std::invalid_argument( "Must specify only one of input or value." );
	}
	if( input )
	{
		result->setInput( input );
	}
	else if( value!=object() )
	{
		typename T::ValueType v = extract<typename T::ValueType>( value )();
		result->setValue( v );
	}
	return result;
}

template<typename T>
static void bind()
{
	typedef typename T::ValueType V;
	
	IECore::RunTimeTypedClass<T>()
		.def( "__init__", make_constructor( construct<T>, default_call_policies(), 
				(
					boost::python::arg_( "name" )=T::staticTypeName(),
					boost::python::arg_( "direction" )=Plug::In,
					boost::python::arg_( "defaultValue" )=V(),
					boost::python::arg_( "flags" )=Plug::None,
					boost::python::arg_( "input" )=PlugPtr( 0 ),
					boost::python::arg_( "value" )=object()
				)
			)
		)
		.def( "defaultValue", &T::defaultValue, return_value_policy<copy_const_reference>() )
		.def( "setValue", &T::setValue )
		.def( "getValue", &T::getValue, return_value_policy<copy_const_reference>() )
	;
	
	Serialiser::registerSerialiser( T::staticTypeId(), serialise<T> );
}

void GafferBindings::bindTypedPlug()
{
	bind<StringPlug>();
	bind<M33fPlug>();
	bind<M44fPlug>();	
}
