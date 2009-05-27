#ifndef GAFFER_SET_INL
#define GAFFER_SET_INL

namespace Gaffer
{

template<typename T>
Set<T>::Set()
{
}

template<typename T>
Set<T>::~Set()
{
}

template<typename T>
IECore::TypeId Set<T>::typeId() const
{
	return staticTypeId();
}

template<typename T>
const char *Set<T>::typeName() const
{
	return staticTypeName();
}

template<typename T>
bool Set<T>::isInstanceOf( IECore::TypeId typeId ) const
{
	if( typeId==staticTypeId() )
	{
		return true;
	}
	return IECore::RunTimeTyped::isInstanceOf( typeId );
}

template<typename T>
bool Set<T>::isInstanceOf( const char *typeName ) const
{
	if( strcmp( typeName, staticTypeName() )==0 )
	{
		return true;
	}
	return IECore::RunTimeTyped::isInstanceOf( typeName );
}

template<typename T>
bool Set<T>::inheritsFrom( IECore::TypeId typeId )
{
	return IECore::RunTimeTyped::staticTypeId()==typeId ? true : IECore::RunTimeTyped::inheritsFrom( typeId );
}

template<typename T>
bool Set<T>::inheritsFrom( const char *typeName )
{
	return strcmp( IECore::RunTimeTyped::staticTypeName(), typeName ) == 0 ? true : IECore::RunTimeTyped::inheritsFrom( typeName );
}

template<typename T>
bool Set<T>::add( typename T::Ptr member )
{
	bool result = m_members.insert( member ).second;
	memberAddedSignal()( this, member );
	return result;
}

template<typename T>
template<typename I>
size_t Set<T>::add( I first, I last )
{
	size_t numAdded = 0;
	for( I it=first; it!=last; it++ )
	{
		ValuePtr v = IECore::runTimeCast<T>( *it );
		if( v )
		{
			numAdded += add( v );
		}
	}
	return numAdded;
}

template<typename T>
bool Set<T>::remove( typename T::Ptr member )
{
	bool result = m_members.erase( member );
	memberRemovedSignal()( this, member );
	return result;
}

template<typename T>
template<typename I>
size_t Set<T>::remove( I first, I last )
{
	size_t numRemoved = 0;
	for( I it=first; it!=last; it++ )
	{
		ValuePtr v = IECore::runTimeCast<T>( *it );
		if( v )
		{
			numRemoved += remove( v );
		}
	}
	return numRemoved;
}

template<typename T>
void Set<T>::clear()
{
	m_members.clear();
}

template<typename T>
bool Set<T>::contains( typename T::ConstPtr object ) const
{
	// const cast is ugly but safe and it allows us to present the
	// appropriate public interface (you should be able to query membership
	// without non-const access to an object).
	return m_members.find( const_cast<T *>( object.get() ) )!=m_members.end();
}

template<typename T>
size_t Set<T>::size() const
{
	return m_members.size();
}

template<typename T>
typename T::Ptr Set<T>::lastAdded()
{
	if( !size() )
	{
		return 0;
	}
	return *(sequencedMembers().rbegin());
}

template<typename T>
typename T::ConstPtr Set<T>::lastAdded() const
{
	if( !size() )
	{
		return 0;
	}
	return *(sequencedMembers().rbegin());
}

template<typename T>
const typename Set<T>::OrderedIndex &Set<T>::members() const
{
	return m_members.template get<0>();
}

template<typename T>
const typename Set<T>::SequencedIndex &Set<T>::sequencedMembers() const
{
	return m_members.template get<1>();
}

template<typename T>
typename Set<T>::MemberSignal &Set<T>::memberAddedSignal()
{
	return m_memberAddedSignal;
}

template<typename T>
typename Set<T>::MemberSignal &Set<T>::memberRemovedSignal()
{
	return m_memberRemovedSignal;
}
				
} // namespace Gaffer

#endif // GAFFER_CONTAINER_INL
