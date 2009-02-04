#include "Gaffer/Node.h"
#include "Gaffer/ScriptNode.h"

#include "boost/bind.hpp"
#include "boost/bind/placeholders.hpp"

using namespace Gaffer;

Node::Node( const std::string &name )
	:	GraphComponent( name )
{
	parentChangedSignal().connect( boost::bind( &Node::parentChanged, this, ::_1 ) );
}

Node::~Node()
{
}

PlugIterator Node::plugsBegin() const
{
	return PlugIterator( children().begin(), children().end() );
}

PlugIterator Node::plugsEnd() const
{
	return PlugIterator( children().end(), children().end() );
}

InputPlugIterator Node::inputPlugsBegin() const
{
	return InputPlugIterator( children().begin(), children().end() );
}

InputPlugIterator Node::inputPlugsEnd() const
{
	return InputPlugIterator( children().end(), children().end() );
}

OutputPlugIterator Node::outputPlugsBegin() const
{
	return OutputPlugIterator( children().begin(), children().end() );
}

OutputPlugIterator Node::outputPlugsEnd() const
{
	return OutputPlugIterator( children().end(), children().end() );
}
		
Node::UnaryPlugSignal &Node::plugSetSignal()
{
	return m_plugSetSignal;
}

Node::UnaryPlugSignal &Node::plugDirtiedSignal()
{
	return m_plugDirtiedSignal;
}

Node::UnaryPlugSignal &Node::plugInputChangedSignal()
{
	return m_plugInputChangedSignal;
}

ScriptNodePtr Node::scriptNode()
{
	return ancestor<ScriptNode>();
}

ConstScriptNodePtr Node::scriptNode() const
{
	return ancestor<ScriptNode>();
}
		
bool Node::acceptsChild( ConstGraphComponentPtr potentialChild ) const
{
	if( !GraphComponent::acceptsChild( potentialChild ) )
	{
		return false;
	}
	return potentialChild->isInstanceOf( (IECore::TypeId)PlugTypeId ) || potentialChild->isInstanceOf( (IECore::TypeId)NodeTypeId );
}

bool Node::acceptsParent( const GraphComponent *potentialParent ) const
{
	if( !GraphComponent::acceptsParent( potentialParent ) )
	{
		return false;
	}
	return potentialParent->isInstanceOf( (IECore::TypeId)NodeTypeId );
}

void Node::parentChanged( GraphComponent *self )
{
	assert( self==this );
	
	for( InputPlugIterator it=inputPlugsBegin(); it!=inputPlugsEnd(); it++ )
	{
		(*it)->setInput( 0 );
	}
	
	for( OutputPlugIterator it=outputPlugsBegin(); it!=outputPlugsEnd(); it++ )
	{
		(*it)->removeOutputs();
	}
}
