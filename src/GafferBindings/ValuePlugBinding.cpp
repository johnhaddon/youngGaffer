#include "boost/python.hpp"

#include "GafferBindings/ValuePlugBinding.h"
#include "Gaffer/ValuePlug.h"
#include "Gaffer/Node.h"

#include "IECore/bindings/Wrapper.h"
#include "IECore/bindings/RunTimeTypedBinding.h"

using namespace boost::python;
using namespace GafferBindings;
using namespace Gaffer;

void GafferBindings::bindValuePlug()
{
	IECore::RunTimeTypedClass<ValuePlug>()
		.def( "setDirty", &ValuePlug::setDirty )
		.def( "getDirty", &ValuePlug::getDirty )
	;	
}
