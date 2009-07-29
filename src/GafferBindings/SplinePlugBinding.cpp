#include "boost/python.hpp"

#include "GafferBindings/SplinePlugBinding.h"
#include "GafferBindings/Serialiser.h"
#include "GafferBindings/PlugBinding.h"
#include "GafferBindings/ValuePlugBinding.h"
#include "Gaffer/Node.h"
#include "Gaffer/SplinePlug.h"
#include "Gaffer/TypedPlug.h"

#include "IECore/bindings/Wrapper.h"
#include "IECore/bindings/RunTimeTypedBinding.h"

using namespace boost::python;
using namespace GafferBindings;
using namespace Gaffer;

template<typename T>
static std::string serialise( Serialiser &s, ConstGraphComponentPtr g )
{
	typename T::Ptr plug = boost::const_pointer_cast<T>( boost::static_pointer_cast<const T>( g ) );
	std::string result = s.modulePath( g ) + "." + g->typeName() + "( \"" + g->getName() + "\", ";
		
	if( plug->direction()!=Plug::In )
	{
		result += "direction = " + serialisePlugDirection( plug->direction() ) + ", ";
	}
	
	object pythonPlug( plug );
	if( plug->defaultValue()!=typename T::ValueType() )
	{
		object pythonValue = pythonPlug.attr( "defaultValue" )();
		s.modulePath( pythonValue );
		std::string value = extract<std::string>( pythonValue.attr( "__repr__" )() );
		result += "defaultValue = " + value + ", ";
	}
	
	if( plug->getFlags() )
	{
		result += "flags = " + serialisePlugFlags( plug->getFlags() ) + ", ";
	}
	
	result += "basisMatrix = " + serialisePlugValue( s, plug->basisMatrixPlug() ) + ", ";
	result += "basisStep = " + serialisePlugValue( s, plug->basisStepPlug() ) + ", ";
	
	unsigned numPoints = plug->numPoints();
	if( numPoints )
	{
		result += "points = ( ";
	
		for( unsigned i=0; i<numPoints; i++ )
		{
			result += "( " + serialisePlugValue( s, plug->pointXPlug( i ) ) + ", " +
				serialisePlugValue( s, plug->pointYPlug( i ) ) + " ), ";
		}
	
		result += "), ";
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
	object basisMatrix,
	object basisStep,
	object points
)
{
	typename T::Ptr result = new T( name, direction, defaultValue, flags );
	
	if( basisMatrix!=object() )
	{
		setPlugValue( result->basisMatrixPlug(), basisMatrix );
	}
	if( basisStep!=object() )
	{
		setPlugValue( result->basisStepPlug(), basisStep );
	}
	
	if( points!=object() )
	{
		result->clearPoints();
		size_t s = extract<size_t>( points.attr( "__len__" )() );
		for( size_t i=0; i<s; i++ )
		{
			tuple t = extract<tuple>( points[i] );
			unsigned pi = result->addPoint();
			setPlugValue( result->pointXPlug( pi ), t[0] );
			setPlugValue( result->pointYPlug( pi ), t[1] );
		}
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
					boost::python::arg_( "basisMatrix" )=object(),
					boost::python::arg_( "basisStep" )=object(),
					boost::python::arg_( "points" )=object()
				)
			)
		)
		.def( "defaultValue", &T::defaultValue, return_value_policy<copy_const_reference>() )
		.def( "setValue", &T::setValue )
		.def( "getValue", &T::getValue )
		.def( "numPoints", &T::numPoints )
		.def( "addPoint", &T::addPoint )
		.def( "removePoint", &T::removePoint )
		.def( "clearPoints", &T::clearPoints )
		.def( "pointPlug", (CompoundPlugPtr (T::*)( unsigned ))&T::pointPlug )
		.def( "pointXPlug", (typename T::XPlugType::Ptr (T::*)( unsigned ))&T::pointXPlug )
		.def( "pointYPlug", (typename T::YPlugType::Ptr (T::*)( unsigned ))&T::pointYPlug )
	;
	
	Serialiser::registerSerialiser( T::staticTypeId(), serialise<T> );
}

void GafferBindings::bindSplinePlug()
{
	bind<SplineffPlug>();
	bind<SplinefColor3fPlug>();
}
