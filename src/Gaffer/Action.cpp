#include "IECore/Exception.h"
#include "IECore/RunTimeTyped.h"

#include "Gaffer/Action.h"
#include "Gaffer/ScriptNode.h"

using namespace Gaffer;

Action::Action()
	:	m_done( false )
{
}

Action::~Action()
{
}

void Action::doAction()
{
	if( m_done ) 
	{
		throw IECore::Exception( "Action cannot be done again without being undone first." );
	}
	m_done = true;
}

void Action::undoAction()
{
	if( !m_done ) 
	{
		throw IECore::Exception( "Action cannot be undone without being done first." );
	}
	
	m_done = false;
}

void Action::addToScript( GraphComponentPtr subject )
{
	doAction();
	ScriptNodePtr s = IECore::runTimeCast<ScriptNode>( subject );
	if( !s )
	{
		s = subject->ancestor<ScriptNode>();
	}
	if( s && s->m_actionAccumulator )
	{
		s->m_actionAccumulator->push_back( this );
	}
}
