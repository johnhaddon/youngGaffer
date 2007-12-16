#include "Gaffer/ValuePlug.h"
#include "Gaffer/Node.h"

#include "boost/format.hpp"

using namespace Gaffer;

ValuePlug::ValuePlug( const std::string &name, Direction direction )
	:	Plug( name, direction ), m_dirty( false )
{
}

ValuePlug::~ValuePlug()
{
}

bool ValuePlug::acceptsInput( ConstPlugPtr input ) const
{
	if( !Plug::acceptsInput( input ) )
	{
		return false;
	}
	return input->isInstanceOf( staticTypeId() );
}

void ValuePlug::setInput( PlugPtr input )
{
	Plug::setInput( input );
	// cast safe because acceptsInput checks type.
	ValuePlugPtr vInput = boost::static_pointer_cast<ValuePlug>( input );
	if( vInput->getDirty() )
	{
		setDirty();
	}
	else
	{
		setFromInput();
	}
}

void ValuePlug::setDirty()
{
	if( m_dirty )
	{
		return;
	}
	if( direction()==In && !getInput<Plug>() )
	{
		throw IECore::Exception( boost::str( boost::format( "Cannot set \"%s\" dirty as it's an input with no incoming connection." ) % fullName() ) );
	}
	m_dirty = true;
	NodePtr n = node();
	if( n )
	{
		n->plugDirtiedSignal()( this );
		if( direction()==In )
		{
			n->dirty( this );
		}
	}
	for( OutputContainer::const_iterator it=outputs().begin(); it!=outputs().end(); it++ )
	{
		ValuePlugPtr o = IECore::runTimeCast<ValuePlug>( *it );
		if( o )
		{
			o->setDirty();
		}
	}
}

bool ValuePlug::getDirty() const
{
	return m_dirty;
}

void ValuePlug::valueSet()
{
	m_dirty = false;
	NodePtr n = node();
	if( n )
	{
		if( direction()==In )
		{
			n->dirty( this );
		}
		n->plugSetSignal()( this );
	}
	for( OutputContainer::const_iterator it=outputs().begin(); it!=outputs().end(); it++ )
	{
		ValuePlugPtr o = IECore::runTimeCast<ValuePlug>( *it );
		if( o )
		{
			o->setFromInput();
		}
	}
}

void ValuePlug::computeIfDirty()
{
	if( getDirty() )
	{
		NodePtr n = node();
		if( !n )
		{
			throw IECore::Exception( boost::str( boost::format( "Unable to compute value for orphan Plug \"%s\"." ) % fullName() ) ); 
		}
		n->compute( this );
	}
}
