#ifndef GAFFER_SCRIPTNODE_H
#define GAFFER_SCRIPTNODE_H

#include "Gaffer/Node.h"
#include "Gaffer/TypedPlug.h"
#include "Gaffer/Container.h"

typedef struct _object PyObject;

namespace Gaffer
{

IE_CORE_FORWARDDECLARE( ScriptNode );

typedef Container<GraphComponent, ScriptNode> ScriptContainer;
IE_CORE_DECLAREPTR( ScriptContainer );

/// The ScriptNode class represents a script - that is a single collection of
/// nodes which are stored in a single file.
/// \todo undo() and redo() methods somehow (i think they probably belong here?)
/// \todo fileValid() plug or summink - bool which is true when the file is saved and
/// false when it's modified in memory. this means attaching a plugchanged callback to every
/// node.
/// \todo Save/load (load should just be clear() followed by executeFile().
class ScriptNode : public Node
{

	public :

		ScriptNode( const std::string &name=staticTypeName() );
		virtual ~ScriptNode();

		IE_CORE_DECLARERUNTIMETYPEDEXTENSION( ScriptNode, ScriptNodeTypeId, Node );
				
		/// Accepts parenting only to a ScriptContainer.
		virtual bool acceptsParent( const GraphComponent *potentialParent ) const;
		
		//! @name Script evaluation
		/// These methods allow the execution of python scripts in the
		/// context of the ScriptNode. The methods are only available on
		/// ScriptNode objects created from Python - they will throw Exceptions
		/// on nodes created from C++. This allows the ScriptNode class to be
		/// used in the C++ library without introducing dependencies on Python.
		/// Exceptions are also thrown if execution fails for any reason.
		////////////////////////////////////////////////////////////////////
		//@{
		typedef boost::signal<void ( ScriptNodePtr, const std::string )> ScriptExecutedSignal;
		typedef boost::signal<void ( ScriptNodePtr, const std::string, PyObject * )> ScriptEvaluatedSignal;
		/// Runs the specified python script.
		virtual void execute( const std::string &pythonScript );
		/// This signal is emitted following successful execution of a script.
		ScriptExecutedSignal &scriptExecutedSignal();
		/// Evaluates the specified python expression. The caller owns a reference to
		/// the result, and must therefore decrement the reference count when
		/// appropriate.
		virtual PyObject *evaluate( const std::string &pythonExpression );
		/// This signal is emitted following sucessful evaluation of an expression. The PyObject *
		/// is the result of the script evaluation - slots must increment the reference count on
		/// this if they intend to keep the result.
		ScriptEvaluatedSignal &scriptEvaluatedSignal();
		//@}
		
		StringPlugPtr fileNamePlug();
		ConstStringPlugPtr fileNamePlug() const;
	
	protected :
	
		virtual void dirty( ConstPlugPtr dirty ) const;
		virtual void compute( PlugPtr output ) const;
			
	private :
	
		ScriptExecutedSignal m_scriptExecutedSignal;
		ScriptEvaluatedSignal m_scriptEvaluatedSignal;
	
		StringPlugPtr m_fileNamePlug;
	
};

IE_CORE_DECLAREPTR( ScriptNode );

} // namespace Gaffer

#endif // GAFFER_SCRIPTNODE_H
