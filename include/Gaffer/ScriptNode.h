#ifndef GAFFER_SCRIPTNODE_H
#define GAFFER_SCRIPTNODE_H

#include "Gaffer/Node.h"
#include "Gaffer/TypedPlug.h"

namespace Gaffer
{

/// The ScriptNode class represents a script - that is a single collection of
/// nodes which are stored in a single file.
/// \todo execute() method.
/// \todo undo() and redo() methods somehow (i think they probably belong here?)
/// \todo fileValid() plug or summink - bool which is true when the file is saved and
/// false when it's modified in memory. this means attaching a plugchanged callback to every
/// node.
class ScriptNode : public Node
{

	public :

		ScriptNode( const std::string &name=staticTypeName() );
		virtual ~ScriptNode();

		IE_CORE_DECLARERUNTIMETYPEDEXTENSION( ScriptNode, ScriptNodeTypeId, Node );
				
		/// Accepts no parent.
		/// \todo In the future it should probably accept parenting to some sort
		/// of ApplicationNode - which represents a whole application. This would
		/// have children which are scripts, global preferences nodes etc. We might
		/// have different ApplicationNode subclasses for different apps - gaffer,
		/// shader builder etc...or we might not...
		virtual bool acceptsParent( const GraphComponent *potentialParent ) const;
		
		StringPlugPtr fileNamePlug();
		ConstStringPlugPtr fileNamePlug() const;
	
	protected :
	
		virtual void dirty( ConstPlugPtr dirty ) const;
		virtual void compute( PlugPtr output ) const;
			
	private :
	
		StringPlugPtr m_fileNamePlug;
	
};

IE_CORE_DECLAREPTR( ScriptNode );

} // namespace Gaffer

#endif // GAFFER_SCRIPTNODE_H
