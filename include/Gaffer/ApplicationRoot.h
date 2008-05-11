#ifndef GAFFER_APPLICATIONROOT_H
#define GAFFER_APPLICATIONROOT_H

#include "Gaffer/ScriptNode.h"

namespace Gaffer
{

class ApplicationRoot : public GraphComponent
{

	public :
	
		ApplicationRoot();
		virtual ~ApplicationRoot();
		
		IE_CORE_DECLARERUNTIMETYPEDEXTENSION( ApplicationRoot, ApplicationRootTypeId, GraphComponent );

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

IE_CORE_DECLAREPTR( ApplicationRoot );

} // namespace Gaffer

#endif // GAFFER_APPLICATIONROOT_H
