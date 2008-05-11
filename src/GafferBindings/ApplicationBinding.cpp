#include "boost/python.hpp"

#include "GafferBindings/ApplicationBinding.h"
#include "Gaffer/Application.h"

#include "IECore/bindings/IntrusivePtrPatch.h"
#include "IECore/bindings/RunTimeTypedBinding.h"

using namespace boost::python;
using namespace GafferBindings;
using namespace Gaffer;

void GafferBindings::bindApplication()
{
	typedef class_<Application, ApplicationPtr, boost::noncopyable, bases<GraphComponent> > ApplicationPyClass;

	ApplicationPyClass( "Application" )
		.IE_COREPYTHON_DEFRUNTIMETYPEDSTATICMETHODS( Application )
	;
	
	INTRUSIVE_PTR_PATCH( Application, ApplicationPyClass );
	implicitly_convertible<ApplicationPtr, GraphComponentPtr>();
	implicitly_convertible<ApplicationPtr, ConstApplicationPtr>();
	
}
