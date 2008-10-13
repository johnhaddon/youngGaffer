#ifndef GAFFER_GRAPHCOMPONENT_INL
#define GAFFER_GRAPHCOMPONENT_INL

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
			if( (*it)->isInstanceOf( T::staticTypeId() ) )
			{
				return static_cast<const T *>( (*it).get() );
			}
		}
	}
	return 0;
}

template<typename T>
FilteredChildIterator<T> GraphComponent::childrenBegin() const
{
	return FilteredChildIterator<T>( m_children.begin(), m_children.end() );
}
		
template<typename T>
FilteredChildIterator<T> GraphComponent::childrenEnd() const
{
	return FilteredChildIterator<T>( m_children.end(), m_children.end() );
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

template<typename T>
typename T::Ptr GraphComponent::ancestor()
{
	return boost::static_pointer_cast<T>( ancestor( T::staticTypeId() ) );
}

template<typename T>
typename T::ConstPtr GraphComponent::ancestor() const
{
	return boost::static_pointer_cast<const T>( ancestor( T::staticTypeId() ) );
}

template<typename T>
typename T::Ptr GraphComponent::commonAncestor( ConstGraphComponentPtr other )
{
	return boost::static_pointer_cast<T>( commonAncestor( other, T::staticTypeId() ) );
}

template<typename T>
typename T::ConstPtr GraphComponent::commonAncestor( ConstGraphComponentPtr other ) const
{
	return boost::static_pointer_cast<const T>( commonAncestor( other, T::staticTypeId() ) );
}
		
} // namespace Gaffer

#endif // GAFFER_GRAPHCOMPONENT_INL
