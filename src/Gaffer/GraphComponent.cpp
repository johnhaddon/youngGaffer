#include "Gaffer/GraphComponent.h"

#include "IECore/Exception.h"

#include "boost/format.hpp"

using namespace Gaffer;
using namespace IECore;
using namespace std;

GraphComponent::GraphComponent( const std::string &name )
	: m_name( name ), m_parent( 0 )
{
}

GraphComponent::~GraphComponent()
{
	// we're dying because our reference count is 0.
	// but the removeChild calls will make a smart pointer
	// from this, to pass to the childRemoved() signal.
	// when that smart pointer goes out of scope, it'll try
	// to kill us again, which is bad. so we add a reference here
	// to prevent the second death. the alternative would be
	// to prevent the emission of the childRemoved signal during
	// death - that might turn out to be a better option.
	/// \todo this needs serious thought. as smart pointers are passed
	/// to the signals, they might reasonably expect to be allowed
	/// to hold onto the object, which doesn't make any sense.
	addRef();
	while( m_children.begin()!=m_children.end() )
	{
		removeChild( *(m_children.begin()) );
	}
}

const std::string &GraphComponent::setName( const std::string &name )
{
	int suffix = 0;
	string newName = name;
	if( m_parent )
	{
		bool unique = true;
		do
		{
			if( suffix!=0 )
			{
				newName = boost::str( boost::format( "%s%d" ) % name % suffix );
				suffix++;
			}
			unique = true;
			for( ChildContainer::const_iterator it=m_parent->m_children.begin(); it!=m_parent->m_children.end(); it++ )
			{
				if( (*it)!=this && (*it)->m_name==newName )
				{
					unique = false;
					break;
				}
			}
			suffix++;
		} while( !unique );
	}

	if( newName==m_name )
	{
		return m_name;
	}
	m_name = newName;
	nameChangedSignal()( this );
	return m_name;
}

const std::string &GraphComponent::getName() const
{
	return m_name;
}

std::string GraphComponent::fullName() const
{
	string fullName = m_name;
	GraphComponent *c = m_parent;
	while( c )
	{
		fullName = c->m_name + "." + fullName;
		c = c->m_parent;
	}
	return fullName;
}

GraphComponent::UnarySignal &GraphComponent::nameChangedSignal()
{
	return m_nameChangedSignal;
}

bool GraphComponent::acceptsChild( ConstGraphComponentPtr potentialChild ) const
{
	return true;
}

bool GraphComponent::acceptsParent( ConstGraphComponentPtr potentialParent ) const
{
	return true;
}

void GraphComponent::addChild( GraphComponentPtr child )
{
	if( !acceptsChild( child ) )
	{
		string what = boost::str( boost::format( "Parent \"%s\" rejects child \"%s\"." ) % m_name % child->m_name );
		throw Exception( what );
	}
	if( !child->acceptsParent( this ) )
	{
		string what = boost::str( boost::format( "Child \"%s\" rejects parent \"%s\"." ) % child->m_name % m_name );
		throw Exception( what );
	}
	if( child->m_parent )
	{
		child->m_parent->removeChild( child );
	}
	m_children.push_back( child );
	child->m_parent = this;
	child->setName( child->m_name ); // to force uniqueness
	childAddedSignal()( this, child );
	child->parentChangedSignal()( child );
}

void GraphComponent::removeChild( GraphComponentPtr child )
{
	if( child->m_parent!=this )
	{
		throw Exception( "Object is not a child." );
	}
	m_children.remove( child );
	child->m_parent = 0;
	childRemovedSignal()( this, child );
	child->parentChangedSignal()( child );
}

const GraphComponent::ChildContainer &GraphComponent::children()
{
	return m_children;
}

GraphComponent::BinarySignal &GraphComponent::childAddedSignal()
{
	return m_childAddedSignal;
}

GraphComponent::BinarySignal &GraphComponent::childRemovedSignal()
{
	return m_childRemovedSignal;
}

GraphComponent::UnarySignal &GraphComponent::parentChangedSignal()
{
	return m_parentChangedSignal;
}
