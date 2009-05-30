#ifndef GAFFER_GRAPHCOMPONENT_INL
#define GAFFER_GRAPHCOMPONENT_INL

#include "boost/tokenizer.hpp"

namespace Gaffer
{

template<typename T>
typename T::Ptr GraphComponent::getChild( const std::string &name )
{
	// preferring the nasty casts over mainaining two nearly identical implementations for getChild.
	return boost::const_pointer_cast<T>( const_cast<const GraphComponent *>( this )->getChild<T>( name ) );
}

template<typename T>
typename T::ConstPtr GraphComponent::getChild( const std::string &name ) const
{
	typedef boost::tokenizer<boost::char_separator<char> > Tokenizer;

	Tokenizer t( name, boost::char_separator<char>( "." ) );
	const GraphComponent *result = this;
	for( Tokenizer::iterator tIt=t.begin(); tIt!=t.end(); tIt++ )
	{
		const GraphComponent *child = 0;
		IECore::InternedString internedName( *tIt );
		for( ChildContainer::const_iterator it=result->m_children.begin(); it!=result->m_children.end(); it++ )
		{
			if( (*it)->m_name==internedName )
			{
				child = it->get();
				break;
			}
		}
		if( !child )
		{
			return 0;
		}
		result = child;
	}
	
	return IECore::runTimeCast<const T>( result );
}

template<typename T>
FilteredChildIterator<TypePredicate<T> > GraphComponent::childrenBegin() const
{
	return FilteredChildIterator<TypePredicate<T> >( m_children.begin(), m_children.end() );
}
		
template<typename T>
FilteredChildIterator<TypePredicate<T> > GraphComponent::childrenEnd() const
{
	return FilteredChildIterator<TypePredicate<T> >( m_children.end(), m_children.end() );
}
		
template<typename T>
T *GraphComponent::parent()
{
	return IECore::runTimeCast<T>( m_parent );
}

template<typename T>
const T *GraphComponent::parent() const
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
