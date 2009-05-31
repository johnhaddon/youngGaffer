#include "boost/python.hpp"

#include "GafferBindings/CompoundPlugBinding.h"
#include "Gaffer/CompoundPlug.h"

#include "IECore/bindings/RunTimeTypedBinding.h"

using namespace boost::python;
using namespace GafferBindings;
using namespace Gaffer;

void GafferBindings::bindCompoundPlug()
{
	IECore::RunTimeTypedClass<CompoundPlug>()
	;	
}
