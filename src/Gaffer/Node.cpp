#include "Gaffer/Node.h"

using namespace Gaffer;

Node::Node( const std::string &name )
	:	GraphComponent( name )
{
}

Node::~Node()
{
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
