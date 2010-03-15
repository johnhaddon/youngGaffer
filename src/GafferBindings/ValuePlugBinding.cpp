#include "boost/python.hpp"

#include "GafferBindings/ValuePlugBinding.h"
#include "GafferBindings/PlugBinding.h"
#include "Gaffer/ValuePlug.h"
#include "Gaffer/Node.h"

#include "IECorePython/Wrapper.h"
#include "IECorePython/RunTimeTypedBinding.h"

using namespace boost::python;
using namespace GafferBindings;
using namespace Gaffer;

std::string GafferBindings::serialisePlugValue( Serialiser &s, PlugPtr plug )
{
	std::string input = serialisePlugInput( s, plug );
	if( input!="" )
	{
		return input;
	}

	if( plug->isInstanceOf( ValuePlug::staticTypeId() ) )
	{
		object pythonPlug( plug );
		object pythonValue = pythonPlug.attr( "getValue" )();
		s.modulePath( pythonValue ); // to get the import statement for the module in the serialisation
		std::string value = extract<std::string>( pythonValue.attr( "__repr__" )() );
		return value;
	}
	
	return "";
}

void GafferBindings::setPlugValue( PlugPtr plug, boost::python::object value )
{
	object pythonPlug( plug );

	extract<PlugPtr> inputExtractor( value );
	if( inputExtractor.check() )
	{
		pythonPlug.attr( "setInput" )( value );
	}
	else
	{
		pythonPlug.attr( "setValue" )( value );
	}			
}

void GafferBindings::bindValuePlug()
{
	IECorePython::RunTimeTypedClass<ValuePlug>()
		.def( "setDirty", &ValuePlug::setDirty )
		.def( "getDirty", &ValuePlug::getDirty )
	;	
}
