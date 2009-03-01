#include "boost/python.hpp"

#include "GafferBindings/ValuePlugBinding.h"
#include "Gaffer/ValuePlug.h"
#include "Gaffer/Node.h"

#include "IECore/bindings/Wrapper.h"
#include "IECore/bindings/WrapperToPython.h"
#include "IECore/bindings/IntrusivePtrPatch.h"
#include "IECore/bindings/RunTimeTypedBinding.h"

using namespace boost::python;
using namespace GafferBindings;
using namespace Gaffer;

void GafferBindings::bindValuePlug()
{
	typedef class_<ValuePlug, ValuePlugPtr, boost::noncopyable, bases<Plug> > ValuePlugPyClass;

	ValuePlugPyClass( "ValuePlug", no_init )
		.def( "setDirty", &ValuePlug::setDirty )
		.def( "getDirty", &ValuePlug::getDirty )
		.IE_COREPYTHON_DEFRUNTIMETYPEDSTATICMETHODS( ValuePlug )
	;

	INTRUSIVE_PTR_PATCH( ValuePlug, ValuePlugPyClass );
	
	implicitly_convertible<ValuePlugPtr, PlugPtr>();
	implicitly_convertible<ValuePlugPtr, ConstValuePlugPtr>();
	
}
