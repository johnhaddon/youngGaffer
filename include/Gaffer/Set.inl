#ifndef GAFFER_SET_INL
#define GAFFER_SET_INL

namespace Gaffer
{

template<typename I>
size_t Set::add( I first, I last )
{
	size_t numAdded = 0;
	for( I it=first; it!=last; it++ )
	{
		numAdded += add( *it );
	}
	return numAdded;
}

template<typename I>
size_t Set::remove( I first, I last )
{
	size_t numRemoved = 0;
	for( I it=first; it!=last; it++ )
	{
		numRemoved += remove( *it );
	}
	return numRemoved;
}

template<typename T>
bool Set::typedMemberAcceptor( Ptr set, ConstMemberPtr potentialMember )
{
	return potentialMember->isInstanceOf( T::staticTypeId() );
}
					
} // namespace Gaffer

#endif // GAFFER_CONTAINER_INL
