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
std::string Set<T>::typeName() const
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
bool Set<T>::isInstanceOf( const std::string &typeName ) const
{
	if( typeName==staticTypeName() )
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
bool Set<T>::inheritsFrom( const std::string &typeName )
{
	return IECore::RunTimeTyped::staticTypeName()==typeName ? true : IECore::RunTimeTyped::inheritsFrom( typeName );
}

template<typename T>
bool Set<T>::add( typename T::Ptr member )
{
	bool result = m_members.insert( member ).second;
	memberAddedSignal()( this, member );
	return result;
}

template<typename T>
bool Set<T>::remove( typename T::Ptr member )
{
	bool result = m_members.erase( member );
	memberRemovedSignal()( this, member );
	return result;
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
