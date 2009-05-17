#include "boost/python.hpp"

#include "GafferBindings/ScriptNodeBinding.h"
#include "GafferBindings/SignalBinding.h"
#include "GafferBindings/Serialiser.h"
#include "Gaffer/ScriptNode.h"

#include "IECore/bindings/Wrapper.h"
#include "IECore/bindings/RunTimeTypedBinding.h"

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
			executionGlobals["deleteNode"] = weakMethod( object( selfO.attr( "deleteNode" ) ) );
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

		virtual std::string serialise( ConstNodeSetPtr filter=0 ) const
		{
			return Serialiser::serialise( this, filter );
		
			/*std::set<ConstNodePtr> visited;
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
			
			return importStatements + "\n" + result;*/
		}
		
		/// \todo Clear the script before executing!!
		/// We need to consider implementing a delete() method first though.
		virtual void load()
		{
			std::string fileName = boost::const_pointer_cast<StringPlug>( fileNamePlug() )->getValue();
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
			
			std::string fileName = boost::const_pointer_cast<StringPlug>( fileNamePlug() )->getValue();
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
			/// from plug names to plug values instead. or pass a tuple of values - one per child?
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
				
				if( !plug->getFlags( Plug::Dynamic ) )
				{
					// we can just serialise the connection/value				
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
				else
				{
					// we need to serialise the whole plug
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

void bindScriptNode()
{
	scope s = IECore::RunTimeTypedClass<ScriptNode, ScriptNodeWrapperPtr>()
		.def( init<>() )
		.def( init<const std::string &>() )
		.def( "selection", (NodeSetPtr (ScriptNode::*)())&ScriptNode::selection )
		.def( "undo", &ScriptNode::undo )
		.def( "redo", &ScriptNode::redo )
		.def( "deleteNode", &ScriptNode::deleteNode )
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
