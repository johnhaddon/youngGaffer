#ifndef GAFFER_SCRIPTNODE_H
#define GAFFER_SCRIPTNODE_H

#include "Gaffer/Node.h"
#include "Gaffer/TypedPlug.h"
#include "Gaffer/Container.h"
#include "Gaffer/Set.h"

typedef struct _object PyObject;

namespace Gaffer
{

IE_CORE_FORWARDDECLARE( ScriptNode );

typedef Container<GraphComponent, ScriptNode> ScriptContainer;
IE_CORE_DECLAREPTR( ScriptContainer );

/// The ScriptNode class represents a script - that is a single collection of
/// nodes which are stored in a single file.
/// \todo undo() and redo() methods somehow (i think they probably belong here?)
/// There should be a Command() class with virtual functions to return the script to be
/// executed on do() and undo(), and these should be held in an undo list. If undo is
/// to be usable from C++ only programs rather than just python, then we need to ensure
/// that the do() and undo() methods don't use execute().
/// \todo fileValid() plug or summink - bool which is true when the file is saved and
/// false when it's modified in memory. this means attaching a plugchanged callback to every
/// node.
class ScriptNode : public Node
{

	public :

		ScriptNode( const std::string &name=staticTypeName() );
		virtual ~ScriptNode();

		IE_CORE_DECLARERUNTIMETYPEDEXTENSION( ScriptNode, ScriptNodeTypeId, Node );
				
		/// Accepts parenting only to a ScriptContainer.
		virtual bool acceptsParent( const GraphComponent *potentialParent ) const;
		
		//! @name Selection
		/// The ScriptNode maintains a list of child Nodes which are considered
		/// to be selected - actions performing on the script can then use that
		/// selection any way they see fit.
		/// \todo We need to figure out how deleted nodes are removed from the
		/// selection. Ideally just doing an unparent would deselect them i
		/// reckon, so del script.nodeName in python would work straight off.
		/// \todo Make sure that selected nodes are appropriate children of
		/// the script.
		////////////////////////////////////////////////////////////////////
		//@{
		NodeSetPtr selection();
		ConstNodeSetPtr selection() const;
		//@}
				
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
		/// \todo I think we'll need a version of this that takes a python callable.
		/// We might expose that here or just introduce it in the binding layer for
		/// use from the python side only.
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
		
	
		//! @name Serialisation
		/// Scripts may be serialised into a string form, which when executed
		/// in python will rebuild the node network.
		/// This process is used for both the saving and loading of scripts and
		/// for the cut and paste mechanism. As serialisation depends on
		/// python, these methods will throw Exceptions if called on ScriptNodes
		/// created from C++.
		/// \todo Very much need to consider how these will work for group
		/// nodes.
		////////////////////////////////////////////////////////////////////
		//@{
		/// Returns a string which when executed will recreate the child nodes
		/// of this script and the connections between them. A NodeSet may be
		/// specified to limit the serialised nodes to those contained in the set.
		virtual std::string serialise( ConstNodeSetPtr filter=0 ) const;
		/// Returns the plug which specifies the file used in all load and save
		/// operations.
		StringPlugPtr fileNamePlug();
		ConstStringPlugPtr fileNamePlug() const;
		/// Loads the script specified in the filename plug.
		virtual void load();
		/// Saves the script to the file specified by the filename plug.
		virtual void save() const;
		//@}
		

	protected :
	
		virtual void dirty( ConstPlugPtr dirty ) const;
		virtual void compute( PlugPtr output ) const;
			
	private :
	
		NodeSetPtr m_selection;
	
		ScriptExecutedSignal m_scriptExecutedSignal;
		ScriptEvaluatedSignal m_scriptEvaluatedSignal;
	
		StringPlugPtr m_fileNamePlug;
	
};

IE_CORE_DECLAREPTR( ScriptNode );

} // namespace Gaffer

#endif // GAFFER_SCRIPTNODE_H
