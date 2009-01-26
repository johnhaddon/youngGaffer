#ifndef GAFFER_UNDOCONTEXT_H
#define GAFFER_UNDOCONTEXT_H

#include "IECore/RefCounted.h"

namespace Gaffer
{

IE_CORE_FORWARDDECLARE( ScriptNode );

class UndoContext
{

	public :

		enum State
		{
			Invalid,
			Enabled,
			Disabled
		};

		/// Script can be 0, in which case the subsequent actions
		/// will not be undoable.
		UndoContext( ScriptNodePtr script, State state=Enabled );
		~UndoContext();

	private :
	
		ScriptNodePtr m_script;
		unsigned m_stateStackSize;

};

} // namespace Gaffer

#endif GAFFER_UNDOCONTEXT_H
