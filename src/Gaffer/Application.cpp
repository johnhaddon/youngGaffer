#include "Gaffer/Application.h"

using namespace Gaffer;

Application::Application()
{
	ScriptContainerPtr s = new ScriptContainer;
	s->setName( "scripts" );
	addChild( s );
}

Application::~Application()
{
}
		
bool Application::acceptsChild( ConstGraphComponentPtr potentialChild ) const
{
	if( children().size()<1 )
	{
		return true;
	}
	return false;
}

bool Application::acceptsParent( const GraphComponent *potentialParent ) const
{
	return false;
}

ScriptContainerPtr Application::scripts()
{
	return getChild<ScriptContainer>( "scripts" );
}

ConstScriptContainerPtr Application::scripts() const
{
	return getChild<ScriptContainer>( "scripts" );
}
