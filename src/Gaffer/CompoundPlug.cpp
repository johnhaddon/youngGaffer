#include "Gaffer/CompoundPlug.h"

using namespace Gaffer;
using namespace boost;

CompoundPlug::CompoundPlug( const std::string &name, Direction direction, unsigned flags )
	:	ValuePlug( name, direction, flags )
{
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
	CompoundPlugPtr p = IECore::runTimeCast<CompoundPlug>( input );
	ChildContainer::const_iterator it1, it2;
	for( it1 = children().begin(), it2 = p->children().begin(); it1!=children().end(); it1++, it2++ )
	{
		static_pointer_cast<Plug>( *it1 )->setInput( static_pointer_cast<Plug>( *it2 ) );
	}
	ValuePlug::setInput( input );
}

void CompoundPlug::setDirty()
{
	ChildContainer::const_iterator it1;
	for( it1 = children().begin(); it1!=children().end(); it1++ )
	{
		ValuePlugPtr p = IECore::runTimeCast<ValuePlug>( *it1 );
		if( p )
		{
			p->setDirty();
		}
	}
	ValuePlug::setDirty();
}
