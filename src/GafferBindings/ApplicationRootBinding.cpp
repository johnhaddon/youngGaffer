#include "boost/python.hpp"

#include "GafferBindings/ApplicationRootBinding.h"
#include "Gaffer/ApplicationRoot.h"

#include "IECore/bindings/RunTimeTypedBinding.h"

using namespace boost::python;
using namespace GafferBindings;
using namespace Gaffer;

void GafferBindings::bindApplicationRoot()
{
	IECore::RunTimeTypedClass<ApplicationRoot>()
		.def( init<>() )
	;	
}
