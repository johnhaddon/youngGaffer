#include "IECore/MessageHandler.h"

#include "Gaffer/UndoContext.h"
#include "Gaffer/ScriptNode.h"
#include "Gaffer/Action.h"

using namespace Gaffer;

UndoContext::UndoContext( ScriptNodePtr script, State state )
	:	m_script( script ), m_stateStackSize( script->m_undoStateStack.size() )
{
	if( state==Invalid )
	{
		throw IECore::Exception( "Cannot construct UndoContext with Invalid state." );
	}
	script->m_undoStateStack.push( state );
	if( m_stateStackSize==0 )
	{
		assert( script->m_actionAccumulator==0 );
		script->m_actionAccumulator = ScriptNode::ActionVectorPtr( new ScriptNode::ActionVector() );
	}
}

UndoContext::~UndoContext()
{
	m_script->m_undoStateStack.pop();
	if( m_script->m_undoStateStack.size()!=m_stateStackSize )
	{
		IECore::msg( IECore::Msg::Warning, "UndoContext::~UndoContext", "Bad undo stack nesting detected" ); 
	}
	if( m_script->m_undoStateStack.size()==0 )
	{
		if( m_script->m_actionAccumulator->size() )
		{
			m_script->m_undoList.erase( m_script->m_undoIterator, m_script->m_undoList.end() );
			m_script->m_undoList.insert( m_script->m_undoList.end(), m_script->m_actionAccumulator );
			m_script->m_undoIterator = m_script->m_undoList.end();
		}
		m_script->m_actionAccumulator = ScriptNode::ActionVectorPtr();
	}
}
