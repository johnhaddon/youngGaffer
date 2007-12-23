#include "boost/python.hpp"

#include "GafferBindings/ScriptNodeBinding.h"
#include "Gaffer/ScriptNode.h"

#include "IECore/bindings/Wrapper.h"
#include "IECore/bindings/WrapperToPython.h"
#include "IECore/bindings/IntrusivePtrPatch.h"

using namespace boost::python;
using namespace GafferBindings;
using namespace Gaffer;

void GafferBindings::bindScriptNode()
{
	typedef class_<ScriptNode, ScriptNodePtr, boost::noncopyable, bases<GraphComponent> > ScriptNodePyClass;

	scope s = ScriptNodePyClass( "ScriptNode" )
		.def( init<const std::string &>() )
	;
		
	INTRUSIVE_PTR_PATCH( ScriptNode, ScriptNodePyClass );
	implicitly_convertible<ScriptNodePtr, NodePtr>();
	implicitly_convertible<ScriptNodePtr, ConstScriptNodePtr>();
	
}
