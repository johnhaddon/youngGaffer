#ifndef GAFFER_APPLICATION_H
#define GAFFER_APPLICATION_H

#include "Gaffer/ScriptNode.h"
#include "Gaffer/Container.h"

namespace Gaffer
{

IE_CORE_FORWARDDECLARE( ScriptNode );

typedef Container<GraphComponent, ScriptNode> ScriptContainer;
GAFFER_DECLARECONTAINERSPECIALISATIONS( ScriptContainer, ScriptContainerTypeId )

IE_CORE_DECLAREPTR( ScriptContainer );

class Application : public GraphComponent
{

	public :
	
		Application();
		virtual ~Application();
		
		IE_CORE_DECLARERUNTIMETYPEDEXTENSION( Application, ApplicationTypeId, GraphComponent );

		/// Accepts no user added children.				
		virtual bool acceptsChild( ConstGraphComponentPtr potentialChild ) const;
		/// Accepts no parent.
		virtual bool acceptsParent( const GraphComponent *potentialParent ) const;
		
		ScriptContainerPtr scripts();
		ConstScriptContainerPtr scripts() const;
		
		/// \todo Implement
		//NodePtr preferences();
		//ConstNodePtr preferences() const;
	
};

IE_CORE_DECLAREPTR( Application );

} // namespace Gaffer

#endif // GAFFER_APPLICATION_H
