#include "boost/python.hpp"

#include "GafferBindings/UndoContextBinding.h"
#include "Gaffer/UndoContext.h"
#include "Gaffer/ScriptNode.h"

using namespace boost::python;
using namespace GafferBindings;
using namespace Gaffer;

namespace GafferBindings
{

void bindUndoContext()
{	
	scope s = class_<UndoContext>( "_UndoContext", init<ScriptNodePtr>() )
		.def( init<ScriptNodePtr, UndoContext::State>() )
	;

	enum_<UndoContext::State>( "State" )
		.value( "Invalid", UndoContext::Invalid )
		.value( "Enabled", UndoContext::Enabled )
		.value( "Disabled", UndoContext::Disabled )
	;
}

} // namespace GafferBindings
