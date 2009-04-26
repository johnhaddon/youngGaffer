#ifndef GAFFER_CONTAINER_INL
#define GAFFER_CONTAINER_INL

namespace Gaffer
{

template<typename Base, typename T>
Container<Base, T>::Container()
{
}

template<typename Base, typename T>
Container<Base, T>::~Container()
{
}		

template<typename Base, typename T>
IECore::TypeId Container<Base, T>::typeId() const
{
	return staticTypeId();
}

template<typename Base, typename T>
const char *Container<Base, T>::typeName() const
{
	return staticTypeName();
}

template<typename Base, typename T>
bool Container<Base, T>::isInstanceOf( IECore::TypeId typeId ) const
{
	if( typeId==staticTypeId() )
	{
		return true;
	}
	return Base::isInstanceOf( typeId );
}

template<typename Base, typename T>
bool Container<Base, T>::isInstanceOf( const char *typeName ) const
{
	if( 0==strcmp( typeName, staticTypeName() ) )
	{
		return true;
	}
	return Base::isInstanceOf( typeName );
}

template<typename Base, typename T>
bool Container<Base, T>::inheritsFrom( IECore::TypeId typeId )
{
	return Base::staticTypeId()==typeId ? true : Base::inheritsFrom( typeId );
}

template<typename Base, typename T>
bool Container<Base, T>::inheritsFrom( const char *typeName )
{
	return 0==strcmp( Base::staticTypeName(), typeName ) ? true : Base::inheritsFrom( typeName );
}

template<typename Base, typename T>
bool Container<Base, T>::acceptsChild( ConstGraphComponentPtr potentialChild ) const
{
	return potentialChild->isInstanceOf( T::staticTypeId() );
}

} // namespace Gaffer

#endif // GAFFER_CONTAINER_INL
