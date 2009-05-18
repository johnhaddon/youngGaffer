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
		
		//! @name Clipboard
		/// The ApplicationRoot class holds a clipboard which is
		/// shared by all ScriptNodes belonging to the application.
		/// The cut, copy and paste methods of the ScriptNodes
		/// operate using this central clipboard. The contents of the
		/// clipboard is simply stored as an IECore::Object.
		////////////////////////////////////////////////////////
		//@{
		/// Returns the clipboard contents, a copy should be taken
		/// if it must be modified.
		IECore::ConstObjectPtr getClipboardContents() const;
		/// Sets the clipboard contents - a copy of clip is taken.
		void setClipboardContents( IECore::ConstObjectPtr clip );
		//@}
		
		/// \todo Implement
		//NodePtr preferences();
		//ConstNodePtr preferences() const;

	private :
	
		IECore::ObjectPtr m_clipboardContents;
	
};

IE_CORE_DECLAREPTR( ApplicationRoot );

} // namespace Gaffer

#endif // GAFFER_APPLICATIONROOT_H
