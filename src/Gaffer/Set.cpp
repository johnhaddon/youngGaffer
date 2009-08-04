#include "Gaffer/Set.h"

using namespace Gaffer;

Set::Set()
{
}

Set::~Set()
{
}

Set::MemberAcceptanceSignal &Set::memberAcceptanceSignal()
{
	return m_memberAcceptanceSignal;
}

bool Set::add( MemberPtr member )
{
	if( !m_memberAcceptanceSignal( this, member ) )
	{
		return false;
	}
	
	bool result = m_members.insert( member ).second;
	if( result )
	{
		memberAddedSignal()( this, member );
	}
	return result;
}

bool Set::remove( MemberPtr member )
{
	bool result = m_members.erase( member );
	if( result )
	{
		memberRemovedSignal()( this, member );
	}
	return result;
}

void Set::clear()
{
	while( m_members.size() )
	{
		remove( *(m_members.begin()) );
	}
}

bool Set::contains( ConstMemberPtr object ) const
{
	// const cast is ugly but safe and it allows us to present the
	// appropriate public interface (you should be able to query membership
	// without non-const access to an object).
	return m_members.find( const_cast<IECore::RunTimeTyped *>( object.get() ) )!=m_members.end();
}

size_t Set::size() const
{
	return m_members.size();
}

Set::MemberPtr Set::lastAdded()
{
	if( !size() )
	{
		return 0;
	}
	return *(sequencedMembers().rbegin());
}

Set::ConstMemberPtr Set::lastAdded() const
{
	if( !size() )
	{
		return 0;
	}
	return *(sequencedMembers().rbegin());
}

const Set::OrderedIndex &Set::members() const
{
	return m_members.get<0>();
}

const Set::SequencedIndex &Set::sequencedMembers() const
{
	return m_members.get<1>();
}

Set::MemberSignal &Set::memberAddedSignal()
{
	return m_memberAddedSignal;
}

Set::MemberSignal &Set::memberRemovedSignal()
{
	return m_memberRemovedSignal;
}
				
