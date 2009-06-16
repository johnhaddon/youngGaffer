#include "boost/bind.hpp"
#include "boost/bind/placeholders.hpp"

#include "Gaffer/CompoundPlug.h"
#include "Gaffer/Node.h"

using namespace Gaffer;
using namespace boost;

CompoundPlug::CompoundPlug( const std::string &name, Direction direction, unsigned flags )
	:	ValuePlug( name, direction, flags )
{
	parentChangedSignal().connect( boost::bind( &CompoundPlug::parentChanged, this ) );
	childAddedSignal().connect( boost::bind( &CompoundPlug::childAddedOrRemoved, this ) );
	childRemovedSignal().connect( boost::bind( &CompoundPlug::childAddedOrRemoved, this ) );
}

CompoundPlug::~CompoundPlug()
{
}

bool CompoundPlug::acceptsChild( ConstGraphComponentPtr potentialChild ) const
{
	ConstPlugPtr p = IECore::runTimeCast<const Plug>( potentialChild );
	if( !p )
	{
		return false;
	}
	return p->direction()==direction();
}

bool CompoundPlug::acceptsInput( ConstPlugPtr input ) const
{
	if( !ValuePlug::acceptsInput( input ) )
	{
		return false;
	}
	
	ConstCompoundPlugPtr p = IECore::runTimeCast<const CompoundPlug>( input );
	if( !p )
	{
		return false;
	}
	if( children().size()!=p->children().size() )
	{
		return false;
	}
	ChildContainer::const_iterator it1, it2;
	for( it1 = children().begin(), it2 = p->children().begin(); it1!=children().end(); it1++, it2++ )
	{
		if( !static_pointer_cast<Plug>( *it1 )->acceptsInput( static_pointer_cast<Plug>( *it2 ) ) )
		{
			return false;
		}
	}
	return true;
}

void CompoundPlug::setInput( PlugPtr input )
{
	if( !input )
	{
		for( ChildContainer::const_iterator it = children().begin(); it!=children().end(); it++ )
		{
			static_pointer_cast<Plug>( *it )->setInput( 0 );			
		}
	}
	else
	{
		CompoundPlugPtr p = static_pointer_cast<CompoundPlug>( input );
		ChildContainer::const_iterator it1, it2;
		for( it1 = children().begin(), it2 = p->children().begin(); it1!=children().end(); it1++, it2++ )
		{
			static_pointer_cast<Plug>( *it1 )->setInput( static_pointer_cast<Plug>( *it2 ) );
		}
	}
	ValuePlug::setInput( input );
}

void CompoundPlug::setDirty()
{
	ChildContainer::const_iterator it;
	for( it = children().begin(); it!=children().end(); it++ )
	{
		ValuePlugPtr p = IECore::runTimeCast<ValuePlug>( *it );
		if( p )
		{
			p->setDirty();
		}
	}
	ValuePlug::setDirty();
}

void CompoundPlug::setFromInput()
{
	// no need to do anything, as our value is stored
	// in the child plugs, and their setFromInput methods
	// will be called anyway when their individual setInput()
	// methods etc get called.
}

void CompoundPlug::parentChanged()
{
	m_plugInputChangedConnection.disconnect();
	m_plugSetConnection.disconnect();
	
	NodePtr n = node();
	if( n )
	{
		m_plugInputChangedConnection = n->plugInputChangedSignal().connect( boost::bind( &CompoundPlug::plugInputChanged, this, ::_1 ) );
		m_plugSetConnection = n->plugSetSignal().connect( boost::bind( &CompoundPlug::plugSet, this, ::_1 ) );
	}
}

void CompoundPlug::childAddedOrRemoved()
{
	updateInputFromChildInputs( 0 );
}

void CompoundPlug::plugInputChanged( PlugPtr plug )
{
	if( plug->parent<CompoundPlug>()==this )
	{
		updateInputFromChildInputs( plug );
	}
}

void CompoundPlug::plugSet( PlugPtr plug )
{
	if( plug->parent<CompoundPlug>()==this )
	{
		NodePtr n = node();
		if( n )
		{
			n->plugSetSignal()( this );
		}
	}
}

void CompoundPlug::updateInputFromChildInputs( PlugPtr checkFirst )
{
	if( !children().size() )
	{
		return;
	}

	if( !checkFirst )
	{
		checkFirst = boost::static_pointer_cast<Plug>( *( children().begin() ) );
	}

	PlugPtr input = checkFirst->getInput<Plug>();	
	if( !input || !input->ancestor<CompoundPlug>() )
	{
		// calling ValuePlug::setInput explicitly rather than setInput
		// so that we don't invoke the behaviour of changing the child
		// plugs' inputs too.
		ValuePlug::setInput( 0 );
		return;
	}
	
	CompoundPlugPtr commonParent = input->ancestor<CompoundPlug>();

	ChildContainer::const_iterator it;
	for( it = children().begin(); it!=children().end(); it++ )
	{
		input = boost::static_pointer_cast<Plug>(*it)->getInput<Plug>();
		if( !input || input->ancestor<CompoundPlug>()!=commonParent )
		{
			ValuePlug::setInput( 0 );
			return;
		}
	}

	ValuePlug::setInput( commonParent );
}
