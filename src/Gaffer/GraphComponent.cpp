#include "Gaffer/GraphComponent.h"
#include "Gaffer/Action.h"

#include "IECore/Exception.h"

#include "boost/format.hpp"
#include "boost/bind.hpp"
#include "boost/regex.hpp"
#include "boost/lexical_cast.hpp"

#include <set>

using namespace Gaffer;
using namespace IECore;
using namespace std;

IE_CORE_DEFINERUNTIMETYPED( GraphComponent );

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

bool GraphComponent::nameExists( const IECore::InternedString &name )
{
	for( ChildContainer::const_iterator it=m_parent->m_children.begin(); it!=m_parent->m_children.end(); it++ )
	{
		if( it->get()!=this && (*it)->m_name==name )
		{
			return true;
		}
	}
	return false;
}

const std::string &GraphComponent::setName( const std::string &name )
{
	// make sure the name is valid
	static boost::regex validator( "^[A-Za-z_]+[A-Za-z0-9]*" );
	if( !regex_match( name.c_str(), validator ) )
	{
		std::string what = boost::str( boost::format( "Invalid name \"%s\"" ) % name );
		throw IECore::Exception( what );
	}
	
	// make sure the name is unique
	IECore::InternedString newName = name;
	if( m_parent )
	{
		if( nameExists( newName ) )
		{
			std::string prefix = newName.value();
			int suffix = 1;
			
			static boost::regex reg( "^(.*[^0-9]+)([0-9]+)$" );
			boost::cmatch match;
			if( regex_match( newName.value().c_str(), match, reg ) )
			{
				prefix = match[1];
				suffix = boost::lexical_cast<int>( match[2] );
			}
			
			do
			{
				static boost::format formatter( "%s%d" );
				newName = boost::str( formatter % prefix % suffix );
				suffix++;
			} while( nameExists( newName ) );	
		}
	}
	
	// set the new name if it's different to the old
	if( newName==m_name.value() )
	{
		return m_name.value();
	}
	
	Action::enact(
		this,
		boost::bind( &GraphComponent::setNameInternal, GraphComponentPtr( this ), newName ),
		boost::bind( &GraphComponent::setNameInternal, GraphComponentPtr( this ), m_name )		
	);
	
	return m_name.value();
}

void GraphComponent::setNameInternal( const IECore::InternedString &name )
{
	m_name = name;
	nameChangedSignal()( this );
}

const std::string &GraphComponent::getName() const
{
	return m_name.value();
}

std::string GraphComponent::fullName() const
{
	return relativeName( 0 );
}

std::string GraphComponent::relativeName( ConstGraphComponentPtr ancestor ) const
{
	string fullName = m_name;
	GraphComponent *c = this->m_parent;
	while( c && c!=ancestor.get() )
	{
		fullName = c->m_name.value() + "." + fullName;
		c = c->m_parent;
	}
	if( ancestor && c!=ancestor.get() )
	{
		string what = boost::str( boost::format( "Object \"%s\" is not an ancestor of \"%s\"." ) % ancestor->m_name.value() % m_name.value() );
		throw Exception( what );
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
		string what = boost::str( boost::format( "Parent \"%s\" rejects child \"%s\"." ) % m_name.value() % child->m_name.value() );
		throw Exception( what );
	}
	if( !child->acceptsParent( this ) )
	{
		string what = boost::str( boost::format( "Child \"%s\" rejects parent \"%s\"." ) % child->m_name.value() % m_name.value() );
		throw Exception( what );
	}

	if( refCount() )
	{
		// someone is pointing to us, so we may have a ScriptNode ancestor and we should do things
		// in an undoable way.
		if( child->m_parent )
		{
			Action::enact(
				this,
				boost::bind( &GraphComponent::addChildInternal, GraphComponentPtr( this ), child ),
				boost::bind( &GraphComponent::addChildInternal, GraphComponentPtr( child->m_parent ), child )		
			);
		}
		else
		{
			Action::enact(
				this,
				boost::bind( &GraphComponent::addChildInternal, GraphComponentPtr( this ), child ),
				boost::bind( &GraphComponent::removeChildInternal, GraphComponentPtr( this ), child )		
			);
		}
	}
	else
	{
		// we have no references to us - chances are we're in construction still. adding ourselves to an
		// undo queue is impossible, and creating temporary smart pointers to ourselves (as above) will
		// cause our destruction before construction completes. just do the work directly.
		addChildInternal( child );
	}
}

void GraphComponent::addChildInternal( GraphComponentPtr child )
{
	if( child->m_parent )
	{
		child->m_parent->removeChildInternal( child );
	}
	m_children.push_back( child );
	child->m_parent = this;
	child->setName( child->m_name.value() ); // to force uniqueness
	childAddedSignal()( this, child.get() );
	child->parentChangedSignal()( child.get() );
}

void GraphComponent::removeChild( GraphComponentPtr child )
{
	if( child->m_parent!=this )
	{
		throw Exception( "Object is not a child." );
	}
	Action::enact(
		this,
		boost::bind( &GraphComponent::removeChildInternal, GraphComponentPtr( this ), child ),
		boost::bind( &GraphComponent::addChildInternal, GraphComponentPtr( this ), child )		
	);
}

void GraphComponent::removeChildInternal( GraphComponentPtr child )
{
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
