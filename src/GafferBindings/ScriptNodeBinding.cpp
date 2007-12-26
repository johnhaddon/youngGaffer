#include "boost/python.hpp"

#include "GafferBindings/ScriptNodeBinding.h"
#include "GafferBindings/SignalBinding.h"
#include "Gaffer/ScriptNode.h"

#include "IECore/bindings/Wrapper.h"
#include "IECore/bindings/WrapperToPython.h"
#include "IECore/bindings/IntrusivePtrPatch.h"
#include "IECore/bindings/RunTimeTypedBinding.h"

using namespace boost::python;
using namespace Gaffer;

namespace GafferBindings
{

/// The ScriptNodeWrapper class implements the scripting
/// components of the ScriptNode base class. In this way
/// scripting is available
class ScriptNodeWrapper : public ScriptNode, public IECore::Wrapper<ScriptNode>
{

	public :

		ScriptNodeWrapper( PyObject *self, const std::string &name=staticTypeName() )
			:	ScriptNode( name ), IECore::Wrapper<ScriptNode>( self, this )
		{
		}

		virtual ~ScriptNodeWrapper()
		{
		}

		virtual void execute( const std::string &pythonScript )
		{
			object mainModule = import( "__main__" );
			object mainNamespace = mainModule.attr( "__dict__" );
			exec( pythonScript.c_str(), mainNamespace, mainNamespace );
			scriptExecutedSignal()( this, pythonScript );
		}

		virtual PyObject *evaluate( const std::string &pythonExpression )
		{
			object mainModule = import( "__main__" );
			object mainNamespace = mainModule.attr( "__dict__" );
			object result = eval( pythonExpression.c_str(), mainNamespace, mainNamespace );
						
			scriptEvaluatedSignal()( this, pythonExpression, result.ptr() );
			
			// make a reference to keep the result alive - the caller then
			// assumes responsibility for dealing with this
			Py_XINCREF( result.ptr() );
			return result.ptr();
		}

};

IE_CORE_DECLAREPTR( ScriptNodeWrapper )

struct ScriptEvaluatedSlotCaller
{
	boost::signals::detail::unusable operator()( boost::python::object slot, ScriptNodePtr node, const std::string script, PyObject *result )
	{
		boost::python::object o( handle<>( borrowed( result ) ) );
		slot( node, script, o );
		return boost::signals::detail::unusable();
	}
};

void bindScriptNode()
{
	typedef class_<ScriptNode, ScriptNodeWrapperPtr, boost::noncopyable, bases<GraphComponent> > ScriptNodePyClass;

	scope s = ScriptNodePyClass( "ScriptNode" )
		.def( init<const std::string &>() )
		.def( "execute", &ScriptNode::execute )
		.def( "evaluate", &ScriptNode::evaluate )
		.def( "scriptExecutedSignal", &ScriptNode::scriptExecutedSignal, return_internal_reference<1>() )
		.def( "scriptEvaluatedSignal", &ScriptNode::scriptEvaluatedSignal, return_internal_reference<1>() )
		.IE_COREPYTHON_DEFRUNTIMETYPEDSTATICMETHODS( ScriptNode )
	;
	
	SignalBinder<ScriptNode::ScriptExecutedSignal>::bind( "ScriptExecutedSignal" );
	SignalBinder<ScriptNode::ScriptEvaluatedSignal, DefaultSignalCaller<ScriptNode::ScriptEvaluatedSignal>, ScriptEvaluatedSlotCaller>::bind( "ScriptEvaluatedSignal" );
	
	IECore::WrapperToPython<ScriptNodePtr>();
	INTRUSIVE_PTR_PATCH( ScriptNode, ScriptNodePyClass );
	implicitly_convertible<ScriptNodePtr, NodePtr>();
	implicitly_convertible<ScriptNodePtr, ConstScriptNodePtr>();
	
}

} // namespace GafferBindings
