#include "Gaffer/ApplicationRoot.h"

using namespace Gaffer;

IE_CORE_DEFINERUNTIMETYPED( ApplicationRoot );

ApplicationRoot::ApplicationRoot()
{
	ScriptContainerPtr s = new ScriptContainer;
	s->setName( "scripts" );
	addChild( s );
}

ApplicationRoot::~ApplicationRoot()
{
}
		
bool ApplicationRoot::acceptsChild( ConstGraphComponentPtr potentialChild ) const
{
	if( children().size()<1 )
	{
		return true;
	}
	return false;
}

bool ApplicationRoot::acceptsParent( const GraphComponent *potentialParent ) const
{
	return false;
}

ScriptContainerPtr ApplicationRoot::scripts()
{
	return getChild<ScriptContainer>( "scripts" );
}

ConstScriptContainerPtr ApplicationRoot::scripts() const
{
	return getChild<ScriptContainer>( "scripts" );
}

IECore::ConstObjectPtr ApplicationRoot::getClipboardContents() const
{
	return m_clipboardContents;
}

void ApplicationRoot::setClipboardContents( IECore::ConstObjectPtr clip )
{
	m_clipboardContents = clip->copy();
}
