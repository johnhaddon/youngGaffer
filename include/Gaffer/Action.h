#ifndef GAFFER_ACTION_H
#define GAFFER_ACTION_H

#include "IECore/RefCounted.h"

namespace Gaffer
{

IE_CORE_FORWARDDECLARE( GraphComponent );
IE_CORE_FORWARDDECLARE( ScriptNode );

/// The Action class forms the basis of the undo system.
class Action : public IECore::RefCounted
{

	protected :

		Action();
		virtual ~Action();
		
		/// Performs the action. Derived classes must
		/// first call the base class implementation.
		/// Throws an IECore::Exception if the action has
		/// already been performed.
		virtual void doAction() = 0;
		/// Undoes the action. Derived classes must first
		/// call the base class implementation. Throws an
		/// IECore::Exception if the action has not been
		/// performed yet.	
		virtual void undoAction() = 0;

		/// Should be called at the end of the most
		/// derived class' constructor. This will call
		/// doAction() and if necessary add the Action
		/// to the undo list of the ScriptNode relevant
		/// to the subject of the action.
		void addToScript( GraphComponentPtr subject );
	
	private :

		friend class ScriptNode;
	
		bool m_done;

};

IE_CORE_DECLAREPTR( Action );

} // namespace Gaffer

#endif // GAFFER_ACTION_H
