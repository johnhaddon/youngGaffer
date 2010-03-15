#include "boost/python.hpp"

#include "GafferBindings/ApplicationRootBinding.h"
#include "Gaffer/ApplicationRoot.h"

#include "IECorePython/RunTimeTypedBinding.h"

using namespace boost::python;
using namespace GafferBindings;
using namespace Gaffer;

static IECore::ObjectPtr getClipboardContents( ApplicationRoot &a )
{
	IECore::ConstObjectPtr o = a.getClipboardContents();
	if( o )
	{
		return o->copy();
	}
	return 0;
}

void GafferBindings::bindApplicationRoot()
{
	IECorePython::RunTimeTypedClass<ApplicationRoot>()
		.def( init<>() )
		.def( "getClipboardContents", &getClipboardContents )
		.def( "setClipboardContents", &ApplicationRoot::setClipboardContents )
	;	
}
