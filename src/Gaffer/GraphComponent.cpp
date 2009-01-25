#include "Gaffer/GraphComponent.h"
#include "Gaffer/Action.h"

#include "IECore/Exception.h"

#include "boost/format.hpp"

#include <set>

using namespace Gaffer;
using namespace IECore;
using namespace std;

GraphComponent::GraphComponent( const std::string &name )
	: m_name( name ), m_parent( 0 )
{
}

GraphComponent::~GraphComponent()
{
	// notify all the children that the parent is gone.
	// we don't call removeChild to achieve this, as that would also emit
	// childRemoved signals for this object, which is undesirable as it's dying.
	for( ChildContainer::iterator it=m_children.begin(); it!=m_children.end(); it++ )
	{
		(*it)->m_parent = 0;
		(*it)->parentChangedSignal()( (*it).get() );
	}	
}

class GraphComponent::SetNameAction : public Action
{
	public :
		SetNameAction( GraphComponentPtr g, const std::string &name )
			:	m_g( g ), m_newName( name ), m_oldName( g->getName() )
		{
			addToScript( g );
		}
		virtual ~SetNameAction()
		{
		}
	protected :
		virtual void doAction()
		{
			Action::doAction();
			m_g->m_name = m_newName;
			m_g->nameChangedSignal()( m_g.get() );
		}
		virtual void undoAction()
		{
			Action::undoAction();
			m_g->m_name = m_oldName;
			m_g->nameChangedSignal()( m_g.get() );		
		}
	private :
		GraphComponentPtr m_g;
		std::string m_newName;
		std::string m_oldName;
};

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
	
	ActionPtr a = new SetNameAction( this, newName );
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

bool GraphComponent::acceptsParent( const GraphComponent *potentialParent ) const
{
	return true;
}

void GraphComponent::addChild( GraphComponentPtr child )
{
	if( child->m_parent==this )
	{
		return;
	}
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
	childAddedSignal()( this, child.get() );
	child->parentChangedSignal()( child.get() );
}

void GraphComponent::removeChild( GraphComponentPtr child )
{
	if( child->m_parent!=this )
	{
		throw Exception( "Object is not a child." );
	}
	m_children.remove( child );
	child->m_parent = 0;
	childRemovedSignal()( this, child.get() );
	child->parentChangedSignal()( child.get() );
}

const GraphComponent::ChildContainer &GraphComponent::children() const
{
	return m_children;
}

GraphComponentPtr GraphComponent::ancestor( IECore::TypeId type )
{
	GraphComponent *a = m_parent;
	while( a )
	{
		if( a->isInstanceOf( type ) )
		{
			return a;
		}
		a = a->m_parent;
	}
	return 0;
}

ConstGraphComponentPtr GraphComponent::ancestor( IECore::TypeId type ) const
{
	return const_cast<GraphComponent *>( this )->ancestor( type );
}

GraphComponentPtr GraphComponent::commonAncestor( ConstGraphComponentPtr other, IECore::TypeId ancestorType )
{
	set<GraphComponent *> candidates;
	GraphComponent *ancestor = m_parent;
	while( ancestor )
	{
		if( ancestor->isInstanceOf( ancestorType ) )
		{
			candidates.insert( ancestor );
		}
		ancestor = ancestor->m_parent;
	}

	ancestor = other->m_parent;
	while( ancestor )
	{
		if( ancestor->isInstanceOf( ancestorType ) )
		{
			if( candidates.find( ancestor )!=candidates.end() )
			{
				return ancestor;
			}
		}
		ancestor = ancestor->m_parent;
	}
	return 0;

}

ConstGraphComponentPtr GraphComponent::commonAncestor( ConstGraphComponentPtr other, IECore::TypeId ancestorType ) const
{
	return const_cast<GraphComponent *>( this )->commonAncestor( other, ancestorType );
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
