#include "IECore/Exception.h"
#include "IECore/RunTimeTyped.h"

#include "Gaffer/Action.h"
#include "Gaffer/ScriptNode.h"

using namespace Gaffer;

Action::Action( const Function &doFn, const Function &undoFn )
	:	m_doFn( doFn ), m_undoFn( undoFn ), m_done( false )
{
}

Action::~Action()
{
}

void Action::enact( GraphComponentPtr subject, const Function &doFn, const Function &undoFn )
{
	ScriptNodePtr s = IECore::runTimeCast<ScriptNode>( subject );
	if( !s )
	{
		s = subject->ancestor<ScriptNode>();
	}
	
	if( s && s->m_actionAccumulator )
	{
		ActionPtr a = new Action( doFn, undoFn );
		a->doAction();
		s->m_actionAccumulator->push_back( a );
	}
	else
	{
		doFn();
	}
	
}
	
void Action::doAction()
{
	if( m_done ) 
	{
		throw IECore::Exception( "Action cannot be done again without being undone first." );
	}
	m_doFn();
	m_done = true;
}

void Action::undoAction()
{
	if( !m_done ) 
	{
		throw IECore::Exception( "Action cannot be undone without being done first." );
	}
	m_undoFn();
	m_done = false;
}

/*void Action::addToScript( GraphComponentPtr subject )
{
}*/
