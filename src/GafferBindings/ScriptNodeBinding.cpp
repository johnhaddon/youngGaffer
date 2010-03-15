#include "boost/python.hpp"

#include "GafferBindings/ScriptNodeBinding.h"
#include "GafferBindings/SignalBinding.h"
#include "GafferBindings/Serialiser.h"
#include "Gaffer/ScriptNode.h"

#include "IECorePython/Wrapper.h"
#include "IECorePython/RunTimeTypedBinding.h"

#include "boost/tokenizer.hpp"

#include <fstream>

using namespace boost::python;
using namespace Gaffer;

namespace GafferBindings
{

/// The ScriptNodeWrapper class implements the scripting
/// components of the ScriptNode base class. In this way
/// scripting is available provided that the ScriptNode was
/// created from python.
class ScriptNodeWrapper : public ScriptNode, public IECorePython::Wrapper<ScriptNode>
{

	public :

		ScriptNodeWrapper( PyObject *self, const std::string &name=staticTypeName() )
			:	ScriptNode( name ), IECorePython::Wrapper<ScriptNode>( self, this )
		{
			dict executionGlobals;
			dict executionLocals;
			
			object builtIn = import( "__builtin__" );
			executionGlobals["__builtins__"] = builtIn;
			
			object gafferModule = import( "Gaffer" );
			executionGlobals["Gaffer"] = gafferModule;
			
			object weakMethod = gafferModule.attr( "WeakMethod" );
			
			object selfO( handle<>( borrowed( self ) ) );
			
			executionGlobals["addChild"] = weakMethod( object( selfO.attr( "addChild" ) ) );
			executionGlobals["getChild"] = weakMethod( object( selfO.attr( "getChild" ) ) );
			executionGlobals["childAddedSignal"] = weakMethod( object( selfO.attr( "childAddedSignal" ) ) );
			executionGlobals["childRemovedSignal"] = weakMethod( object( selfO.attr( "childRemovedSignal" ) ) );
			executionGlobals["selection"] = weakMethod( object( selfO.attr( "selection" ) ) );
			executionGlobals["undo"] = weakMethod( object( selfO.attr( "undo" ) ) );
			executionGlobals["redo"] = weakMethod( object( selfO.attr( "redo" ) ) );
			executionGlobals["deleteNodes"] = weakMethod( object( selfO.attr( "deleteNodes" ) ) );
			executionGlobals["serialise"] = weakMethod( object( selfO.attr( "serialise" ) ) );
			executionGlobals["save"] = weakMethod( object( selfO.attr( "save" ) ) );
			executionGlobals["load"] = weakMethod( object( selfO.attr( "load" ) ) );
			
			// ideally we'd just store the execution scopes as normal
			// c++ member variables but we can't as they may hold
			// references back to ourselves. by storing them in self.__dict__
			// we allow them to participate in garbage collection, thus breaking
			// the cycle and allowing the ScriptNode to die.
			object selfDict = selfO.attr( "__dict__" );
			selfDict["__executionGlobals"] = executionGlobals;
			selfDict["__executionLocals"] = executionLocals;
			
		}

		virtual ~ScriptNodeWrapper()
		{
		}

		virtual void execute( const std::string &pythonScript )
		{
			exec( pythonScript.c_str(), executionGlobals(), executionLocals() );
			scriptExecutedSignal()( this, pythonScript );
		}

		virtual PyObject *evaluate( const std::string &pythonExpression )
		{
			object result = eval( pythonExpression.c_str(), executionGlobals(), executionLocals() );
			scriptEvaluatedSignal()( this, pythonExpression, result.ptr() );
			
			// make a reference to keep the result alive - the caller then
			// assumes responsibility for dealing with this
			Py_XINCREF( result.ptr() );
			return result.ptr();
		}

		virtual std::string serialise( ConstSetPtr filter=0 ) const
		{
			return Serialiser::serialise( this, filter );
		}
		
		/// \todo Clear the script before executing!!
		/// We need to consider implementing a delete() method first though.
		virtual void load()
		{
			std::string fileName = IECore::constPointerCast<StringPlug>( fileNamePlug() )->getValue();
			std::ifstream f( fileName.c_str() );
			if( !f.good() )
			{
				throw IECore::IOException( "Unable to open file \"" + fileName + "\"" );
			}
			
			std::string s;
			while( !f.eof() )
			{
				if( !f.good() )
				{
					throw IECore::IOException( "Failed to read from \"" + fileName + "\"" );
				}

				std::string line;
				std::getline( f, line );
				s += line + "\n";
			}
						
			execute( s );
		}
		
		virtual void save() const
		{
			std::string s = serialise();
			
			std::string fileName = IECore::constPointerCast<StringPlug>( fileNamePlug() )->getValue();
			std::ofstream f( fileName.c_str() );
			if( !f.good() )
			{
				throw IECore::IOException( "Unable to open file \"" + fileName + "\"" );
			}
			
			f << s;
			
			if( !f.good() )
			{
				throw IECore::IOException( "Failed to write to \"" + fileName + "\"" );
			}
		}
		
	private :
	
		object executionGlobals()
		{
			object selfO( handle<>( borrowed( m_pyObject ) ) );
			object selfDict = selfO.attr( "__dict__" );
			return selfDict["__executionGlobals"];
		}
		
		object executionLocals()
		{
			object selfO( handle<>( borrowed( m_pyObject ) ) );
			object selfDict = selfO.attr( "__dict__" );
			return selfDict["__executionLocals"];
		}
		
};

IE_CORE_DECLAREPTR( ScriptNodeWrapper )

struct ScriptEvaluatedSlotCaller
{
	boost::signals::detail::unusable operator()( boost::python::object slot, ScriptNodePtr node, const std::string script, PyObject *result )
	{
		try
		{
			boost::python::object o( handle<>( borrowed( result ) ) );
			slot( node, script, o );
		}
		catch( const error_already_set &e )
		{
			PyErr_PrintEx( 0 ); // clears error status
		}
		return boost::signals::detail::unusable();
	}
};

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS( serialiseOverloads, serialise, 0, 1 );
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS( copyOverloads, copy, 0, 1 );
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS( cutOverloads, cut, 0, 1 );
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS( deleteNodesOverloads, deleteNodes, 0, 1 );

void bindScriptNode()
{
	scope s = IECorePython::RunTimeTypedClass<ScriptNode, ScriptNodeWrapperPtr>()
		.def( init<>() )
		.def( init<const std::string &>() )
		.def( "selection", (SetPtr (ScriptNode::*)())&ScriptNode::selection )
		.def( "undo", &ScriptNode::undo )
		.def( "redo", &ScriptNode::redo )
		.def( "copy", &ScriptNode::copy, copyOverloads() )
		.def( "cut", &ScriptNode::cut, cutOverloads() )
		.def( "paste", &ScriptNode::paste )
		.def( "deleteNodes", &ScriptNode::deleteNodes, deleteNodesOverloads() )
		.def( "execute", &ScriptNode::execute )
		.def( "evaluate", &ScriptNode::evaluate )
		.def( "scriptExecutedSignal", &ScriptNode::scriptExecutedSignal, return_internal_reference<1>() )
		.def( "scriptEvaluatedSignal", &ScriptNode::scriptEvaluatedSignal, return_internal_reference<1>() )
		.def( "serialise", &ScriptNode::serialise, serialiseOverloads() )
		.def( "save", &ScriptNode::save )
		.def( "load", &ScriptNode::load )
	;
	
	SignalBinder<ScriptNode::ScriptExecutedSignal>::bind( "ScriptExecutedSignal" );
	SignalBinder<ScriptNode::ScriptEvaluatedSignal, DefaultSignalCaller<ScriptNode::ScriptEvaluatedSignal>, ScriptEvaluatedSlotCaller>::bind( "ScriptEvaluatedSignal" );	
}

} // namespace GafferBindings
