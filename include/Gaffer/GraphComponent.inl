#ifndef GAFFER_GRAPHCOMPONENT_INL
#define GAFFER_GRAPHCOMPONENT_INL

/// \todo Move me to IECore
namespace IECore
{

template<typename T>
T *runTimeCast( RunTimeTyped *src )
{
	if( !src )
	{
		return 0;
	}
	return src->isInstanceOf( T::staticTypeId() ) ? static_cast<T *>( src ) : 0;
}

}

namespace Gaffer
{

template<typename T>
typename T::Ptr GraphComponent::getChild( const std::string &name )
{
	/// \todo is a linear search going to cut it for many children? perhaps we should map
	/// name->child when there are large numbers of children?
	for( ChildContainer::const_iterator it=m_children.begin(); it!=m_children.end(); it++ )
	{
		if( (*it)->getName()==name )
		{
			return IECore::runTimeCast<T>( *it );
		}
	}
	return 0;
}

template<typename T>
typename T::ConstPtr GraphComponent::getChild( const std::string &name ) const
{
	for( ChildContainer::const_iterator it=m_children.begin(); it!=m_children.end(); it++ )
	{
		if( (*it)->getName()==name )
		{
			return IECore::runTimeCast<const T>( *it );
		}
	}
	return 0;
}

template<typename T>
typename T::Ptr GraphComponent::parent()
{
	return IECore::runTimeCast<T>( m_parent );
}

template<typename T>
typename T::ConstPtr GraphComponent::parent() const
{
	return IECore::runTimeCast<const T>( m_parent );
}

} // namespace Gaffer

#endif // GAFFER_GRAPHCOMPONENT_INL
