#include "boost/python.hpp"

#include "GafferBindings/ApplicationRootBinding.h"
#include "Gaffer/ApplicationRoot.h"

#include "IECore/bindings/IntrusivePtrPatch.h"
#include "IECore/bindings/RunTimeTypedBinding.h"

using namespace boost::python;
using namespace GafferBindings;
using namespace Gaffer;

void GafferBindings::bindApplicationRoot()
{
	typedef class_<ApplicationRoot, ApplicationRootPtr, boost::noncopyable, bases<GraphComponent> > ApplicationRootPyClass;

	ApplicationRootPyClass( "ApplicationRoot" )
		.IE_COREPYTHON_DEFRUNTIMETYPEDSTATICMETHODS( ApplicationRoot )
	;
	
	INTRUSIVE_PTR_PATCH( ApplicationRoot, ApplicationRootPyClass );
	implicitly_convertible<ApplicationRootPtr, GraphComponentPtr>();
	implicitly_convertible<ApplicationRootPtr, ConstApplicationRootPtr>();
	
}
