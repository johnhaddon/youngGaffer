#include "boost/python.hpp"
#include "boost/lexical_cast.hpp"

#include "GafferBindings/CompoundNumericPlugBinding.h"
#include "GafferBindings/PlugBinding.h"
#include "GafferBindings/ValuePlugBinding.h"
#include "GafferBindings/Serialiser.h"
#include "Gaffer/CompoundNumericPlug.h"

#include "IECorePython/RunTimeTypedBinding.h"

using namespace boost::python;
using namespace GafferBindings;
using namespace Gaffer;

template<typename T>
static std::string serialiseValue( Serialiser &s, const T &value )
{
	object pythonValue( value );
	s.modulePath( pythonValue );
	return extract<std::string>( pythonValue.attr( "__repr__" )() );
}

template<typename T>
static std::string serialise( Serialiser &s, ConstGraphComponentPtr g )
{
	typename T::ConstPtr plug = IECore::staticPointerCast<const T>( g );
	std::string result = s.modulePath( g ) + "." + g->typeName() + "( \"" + g->getName() + "\", ";
	
	if( plug->direction()!=Plug::In )
	{
		result += "direction = " + serialisePlugDirection( plug->direction() ) + ", ";
	}
	
	if( plug->defaultValue()!=typename T::ValueType() )
	{
		result += "defaultValue = " + serialiseValue( s, plug->defaultValue() ) + ", ";
	}
	
	if( plug->hasMinValue() )
	{
		result += "minValue = " + serialiseValue( s, plug->minValue() ) + ", ";
	}
	
	if( plug->hasMaxValue() )
	{
		result += "maxValue = " + serialiseValue( s, plug->maxValue() ) + ", ";
	}
	
	if( plug->getFlags() )
	{
		result += "flags = " + serialisePlugFlags( plug->getFlags() ) + ", ";
	}
	
	std::string value = "( ";
	PlugIterator pIt( plug->children().begin(), plug->children().end() );
	while( pIt!=plug->children().end() )
	{
		value += serialisePlugValue( s, IECore::staticPointerCast<ValuePlug>( *pIt++ ) ) + ", ";
	}
	value += " )";
	result += "value = " + value + ", ";
	
	result += ")";

	return result;
}

template<typename T>
static typename T::Ptr construct(
	const char *name,
	Plug::Direction direction,
	typename T::ValueType defaultValue,
	typename T::ValueType minValue,
	typename T::ValueType maxValue,
	unsigned flags,
	object value
)
{
	typename T::Ptr result = new T( name, direction, defaultValue, minValue, maxValue, flags );
	if( value!=object() )
	{
		extract<typename T::ValueType> valueExtractor( value );
		if( valueExtractor.check() )
		{
			typename T::ValueType v = valueExtractor();
			result->setValue( v );
		}
		else
		{
			tuple t = extract<tuple>( value )();
			size_t l = extract<size_t>( t.attr( "__len__" )() )();
			if( l!=T::ValueType::dimensions() )
			{
				PyErr_SetString( PyExc_ValueError, "Wrong number of items in value tuple." );			
				throw_error_already_set();
			}
			size_t i = 0;
			PlugIterator pIt( result->children().begin(), result->children().end() );
			while( pIt!=result->children().end() )
			{
				setPlugValue( IECore::staticPointerCast<ValuePlug>( *pIt++ ), t[i++] );
			}
		}
	}
	return result;
}

template<typename T>
static void bind()
{
	typedef typename T::ValueType V;
		
	IECorePython::RunTimeTypedClass<T>()
		.def( "__init__", make_constructor( construct<T>, default_call_policies(),
				(
					boost::python::arg_( "name" )=T::staticTypeName(),
					boost::python::arg_( "direction" )=Plug::In,
					boost::python::arg_( "defaultValue" )=V( 0 ),
					boost::python::arg_( "minValue" )=V(Imath::limits<typename V::BaseType>::min()),
					boost::python::arg_( "maxValue" )=V(Imath::limits<typename V::BaseType>::max()),
					boost::python::arg_( "flags" )=Plug::None,
					boost::python::arg_( "value" )=object()
				)
			)
		)
		.def( "defaultValue", &T::defaultValue )
		.def( "hasMinValue", &T::hasMinValue )
		.def( "hasMaxValue", &T::hasMaxValue )
		.def( "minValue", &T::minValue )
		.def( "maxValue", &T::maxValue )
		.def( "setValue", &T::setValue )
		.def( "getValue", &T::getValue )
	;

	Serialiser::registerSerialiser( T::staticTypeId(), serialise<T> );

}

void GafferBindings::bindCompoundNumericPlug()
{
	bind<V2fPlug>();
	bind<V3fPlug>();
	bind<V2iPlug>();
	bind<V3iPlug>();
	bind<Color3fPlug>();
	bind<Color4fPlug>();
}
