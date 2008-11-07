#include "boost/python.hpp"

#include "GafferBindings/ScriptNodeBinding.h"
#include "GafferBindings/SignalBinding.h"
#include "Gaffer/ScriptNode.h"

#include "IECore/bindings/Wrapper.h"
#include "IECore/bindings/WrapperToPython.h"
#include "IECore/bindings/IntrusivePtrPatch.h"
#include "IECore/bindings/RunTimeTypedBinding.h"

#include "boost/tokenizer.hpp"

using namespace boost::python;
using namespace Gaffer;

namespace GafferBindings
{

/// The ScriptNodeWrapper class implements the scripting
/// components of the ScriptNode base class. In this way
/// scripting is available provided that the ScriptNode was
/// created from python.
class ScriptNodeWrapper : public ScriptNode, public IECore::Wrapper<ScriptNode>
{

	public :

		ScriptNodeWrapper( PyObject *self, const std::string &name=staticTypeName() )
			:	ScriptNode( name ), IECore::Wrapper<ScriptNode>( self, this )
		{
			dict executionGlobals;
			dict executionLocals;
			
			object builtIn = import( "__builtin__" );
			
			executionGlobals["__builtins__"] = builtIn;
			executionGlobals["Gaffer"] = import( "Gaffer" );
			
			object selfO( handle<>( borrowed( self ) ) );
			executionGlobals["addChild"] = selfO.attr( "addChild" );
			
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
			std::cerr << "SCRIPTNODE DYING!" << std::endl;
		}

		virtual void execute( const std::string &pythonScript )
		{
			//object mainModule = import( "__main__" );
			//object mainNamespace = mainModule.attr( "__dict__" );
			exec( pythonScript.c_str(), executionGlobals(), executionLocals() );
			scriptExecutedSignal()( this, pythonScript );
		}

		virtual PyObject *evaluate( const std::string &pythonExpression )
		{
			//object mainModule = import( "__main__" );
			//object mainNamespace = mainModule.attr( "__dict__" );
			object result = eval( pythonExpression.c_str(), executionGlobals(), executionLocals() );
						
			scriptEvaluatedSignal()( this, pythonExpression, result.ptr() );
			
			// make a reference to keep the result alive - the caller then
			// assumes responsibility for dealing with this
			Py_XINCREF( result.ptr() );
			return result.ptr();
		}

		std::string serialise( ConstNodeSetPtr filter=0 ) const
		{
			std::set<ConstNodePtr> visited;
			std::string result;
			std::set<std::string> moduleDependencies;
			
			ChildNodeIterator nIt;
			for( nIt=childrenBegin<Node>(); nIt!=childrenEnd<Node>(); nIt++ )
			{
				serialiseNode( result, *nIt, filter, visited, moduleDependencies );
			}
			
			std::string importStatements;
			for( std::set<std::string>::const_iterator it=moduleDependencies.begin(); it!=moduleDependencies.end(); it++ )
			{
				importStatements += "import " + *it + "\n";
			}
			
			return importStatements + "\n" + result;
		}
		
	private :
	
		void serialiseNode( std::string &result, ConstNodePtr node, ConstNodeSetPtr filter, std::set<ConstNodePtr> &visited, std::set<std::string> &moduleDependencies ) const
		{
			// early out if the node isn't wanted or we've done it already
			if( filter && !filter->contains( node ) )
			{
				return;
			}
			if( visited.find( node )!=visited.end() )
			{
				return;
			}
			
			// figure out the node type name and module path.
			object pythonNode( boost::const_pointer_cast<Node>( node ) ); // we can only push non-const objects to python so we need the cast
			std::string className = extract<std::string>( pythonNode.attr( "__class__" ).attr( "__name__" ) );
			std::string modulePath = extract<std::string>( pythonNode.attr( "__module__" ) );
			
			typedef boost::tokenizer<boost::char_separator<char> > Tokenizer;
			std::string sanitisedModulePath;
			Tokenizer tokens( modulePath, boost::char_separator<char>( "." ) );
			for( Tokenizer::iterator tIt=tokens.begin(); tIt!=tokens.end(); tIt++ )
			{
				if( tIt->compare( 0, 1, "_" )==0 )
				{
					// assume that module path components starting with _ are bogus, and are used only to bring
					// binary components into a namespace.
					continue;
				}
				Tokenizer::iterator next = tIt; next++;
				if( next==tokens.end() && *tIt == className )
				{
					// if the last module name is the same as the class name then assume this is just the file the
					// class has been implemented in.
					continue;
				}
				if( sanitisedModulePath.size() )
				{
					sanitisedModulePath += ".";
				}
				sanitisedModulePath += *tIt;
			}
			
			moduleDependencies.insert( sanitisedModulePath );
			
			std::string s = node->getName() + " = " + sanitisedModulePath + "." + className + "( \"" + node->getName() + "\",\n";
			
			/// \todo We'll have a problem when we add CompoundPlugs, as we can't pass something.something
			/// names as keyword arguments to the constructors. perhaps we'll have to pass a dictionary mapping
			/// from plug names to plug values instead.
			for( InputPlugIterator pIt=node->inputPlugsBegin(); pIt!=pIt.end(); pIt++ )
			{
				PlugPtr plug = *pIt;
				bool connect = false;
				PlugPtr srcPlug = plug->getInput<Plug>();
				if( srcPlug && srcPlug->node() && srcPlug->node()->scriptNode()==this )
				{
					if( !filter || filter->contains( srcPlug->node() ) )
					{
						connect = true;
					}
				}
				
				if( connect )
				{
					serialiseNode( result, srcPlug->node(), filter, visited, moduleDependencies );
					s += "\t" + plug->getName() + " = " + srcPlug->node()->getName() + "[\"" + srcPlug->getName() + "\"],\n";
				}
				else
				{
					if( plug->isInstanceOf( ValuePlug::staticTypeId() ) )
					{
						object pythonPlug( plug );
						object pythonValue = pythonPlug.attr( "getValue" )();
						std::string value = extract<std::string>( pythonValue.attr( "__repr__" )() );
					 	s += "\t" + plug->getName() + " = " + value + ",\n";
					}
				}
			}
			
			s += ")\n";
			s += "addChild( " + node->getName() + " )\n\n";
			
			result += s;
			
			visited.insert( node );
		}
		
		object executionGlobals()
		{
			object selfO( handle<>( borrowed( m_pyObject ) ) );
			return selfO["__executionGlobals"];
		}
		
		object executionLocals()
		{
			object selfO( handle<>( borrowed( m_pyObject ) ) );
			return selfO["__executionLocals"];
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

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS( serialiseOverloads, serialise, 0, 1 );

void bindScriptNode()
{
	typedef class_<ScriptNode, ScriptNodeWrapperPtr, boost::noncopyable, bases<GraphComponent> > ScriptNodePyClass;

	scope s = ScriptNodePyClass( "ScriptNode" )
		.def( init<const std::string &>() )
		.def( "selection", (NodeSetPtr (ScriptNode::*)())&ScriptNode::selection )
		.def( "execute", &ScriptNode::execute )
		.def( "evaluate", &ScriptNode::evaluate )
		.def( "scriptExecutedSignal", &ScriptNode::scriptExecutedSignal, return_internal_reference<1>() )
		.def( "scriptEvaluatedSignal", &ScriptNode::scriptEvaluatedSignal, return_internal_reference<1>() )
		.def( "serialise", &ScriptNode::serialise, serialiseOverloads() )
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
