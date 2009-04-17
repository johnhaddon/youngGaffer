#include "Gaffer/Plug.h"
#include "Gaffer/Node.h"
#include "Gaffer/Action.h"
#include "Gaffer/ScriptNode.h"

#include "IECore/Exception.h"

#include "boost/format.hpp"
#include "boost/bind.hpp"

using namespace Gaffer;

Plug::Plug( const std::string &name, Direction direction, unsigned flags, PlugPtr input )
	:	GraphComponent( name ), m_direction( direction ), m_input( 0 ), m_flags( flags )
{
	setInput( input );
}

Plug::~Plug()
{
	setInputInternal( 0, false );
	for( OutputContainer::iterator it=m_outputs.begin(); it!=m_outputs.end(); it++ )
	{
		(*it)->setInputInternal( 0, true );
	}
}

bool Plug::acceptsChild( ConstGraphComponentPtr potentialChild ) const
{
	return false;
}

bool Plug::acceptsParent( const GraphComponent *potentialParent ) const
{
	if( !GraphComponent::acceptsParent( potentialParent ) )
	{
		return false;
	}
	return potentialParent->isInstanceOf( (IECore::TypeId)NodeTypeId );
}

NodePtr Plug::node()
{
	return ancestor<Node>();
}


ConstNodePtr Plug::node() const
{
	return ancestor<Node>();
}

Plug::Direction Plug::direction() const
{
	return m_direction;
}

unsigned Plug::getFlags() const
{
	return m_flags;
}

bool Plug::getFlags( unsigned flags ) const
{
	return (m_flags & flags) == flags;
}

void Plug::setFlags( unsigned flags )
{
	m_flags = flags;
}

void Plug::setFlags( unsigned flags, bool enable )
{
	m_flags = (m_flags & ~flags) | ( enable ? flags : 0 );
}

bool Plug::acceptsInput( ConstPlugPtr input ) const
{
	/// \todo Possibly allow in->out connections as long
	/// as the Plugs share the same parent (for internal shortcuts).
	return m_direction!=Out;
}

void Plug::setInput( PlugPtr input )
{
	if( input.get()==m_input )
	{
		return;
	}
	if( input && !acceptsInput( input ) )
	{
		std::string what = boost::str( boost::format( "Plug \"%s\" rejects input \"%s\"." ) % fullName() % input->fullName() );
		throw IECore::Exception( what );
	}
	if( refCount() )
	{
		// someone is referring to us, so we're definitely fully constructed and we may have a ScriptNode
		// above us, so we should do things in a way compatible with the undo system.			
		Action::enact(
			this,
			boost::bind( &Plug::setInputInternal, PlugPtr( this ), input, true ),
			boost::bind( &Plug::setInputInternal, PlugPtr( this ), PlugPtr( m_input ), true )		
		);
	}
	else
	{
		// noone is referring to us. we're probably still constructing, and undo is impossible anyway (we
		// have no ScriptNode ancestor), so we can't make a smart pointer
		// to ourselves (it will result in double destruction). so we just set the input directly.
		setInputInternal( input, false );
	}
}

void Plug::setInputInternal( PlugPtr input, bool emit )
{
	if( m_input )
	{
		m_input->m_outputs.remove( this );
	}
	m_input = input.get();
	if( m_input )
	{
		m_input->m_outputs.push_back( this );
	}
	if( emit )
	{
		NodePtr n = node();
		if( n )
		{
			node()->plugInputChangedSignal()( this );
		}
	}
}

void Plug::removeOutputs()
{
	for( OutputContainer::iterator it = m_outputs.begin(); it!=m_outputs.end();  )
	{
		Plug *p = *it++;
		p->setInput( 0 );
	}
}

const Plug::OutputContainer &Plug::outputs() const
{
	return m_outputs;
}
