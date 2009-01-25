#ifndef GAFFER_ACTION_H
#define GAFFER_ACTION_H

#include "boost/function.hpp"

#include "IECore/RefCounted.h"

namespace Gaffer
{

IE_CORE_FORWARDDECLARE( GraphComponent );
IE_CORE_FORWARDDECLARE( ScriptNode );

/// The Action class forms the basis of the undo system.
class Action : public IECore::RefCounted
{

	public :
	
		typedef boost::function<void ()> Function;

		static void enact( GraphComponentPtr subject, const Function &doFn, const Function &undoFn );

	protected :

		Action( const Function &doFn, const Function &undoFn );
		virtual ~Action();
		
		void doAction();
		void undoAction();

	private :

		friend class ScriptNode;

		Function m_doFn;
		Function m_undoFn;
	
		bool m_done;

};

IE_CORE_DECLAREPTR( Action );

} // namespace Gaffer

#endif // GAFFER_ACTION_H
