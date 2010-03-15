#ifndef GAFFER_FILTEREDCHILDITERATOR_H
#define GAFFER_FILTEREDCHILDITERATOR_H

#include "Gaffer/GraphComponent.h"

#include "IECore/RunTimeTyped.h"

#include "boost/iterator/filter_iterator.hpp"

namespace Gaffer
{

/// \todo Perhaps this predicate class belongs in cortex?
template<typename T>
struct TypePredicate
{
	typedef T ChildType;

	bool operator()( IECore::RunTimeTypedPtr x )
	{
		return IECore::runTimeCast<T>( x );
	}
};

template<typename Predicate>
class FilteredChildIterator : public boost::filter_iterator<Predicate, GraphComponent::ChildIterator>
{
	
	public :
	
		typedef typename Predicate::ChildType ChildType;
		typedef boost::filter_iterator<Predicate, GraphComponent::ChildIterator> BaseIterator;
		
		typedef const typename ChildType::Ptr &reference;
		typedef const typename ChildType::Ptr *pointer;
		
		FilteredChildIterator()
			:	BaseIterator()
		{
		}

    	FilteredChildIterator( GraphComponent::ChildIterator x, GraphComponent::ChildIterator end = GraphComponent::ChildIterator() )
			:	BaseIterator( x, end )
		{
		}

		bool operator==( const GraphComponent::ChildIterator &rhs ) const
		{
			return BaseIterator::base()==( rhs );
		}

		bool operator!=( const GraphComponent::ChildIterator &rhs ) const
		{
			return BaseIterator::base()!=( rhs );
		}
		
		reference operator*() const
		{
			// cast should be safe as predicate has checked type, and the layout of
			// a GraphComponentPtr and any other intrusive pointer should be the same.
			return reinterpret_cast<reference>( BaseIterator::operator*() );
		}

		pointer operator->() const
		{
			return reinterpret_cast<pointer>( BaseIterator::operator->() );
		}
		
    	FilteredChildIterator &operator++()
		{
			BaseIterator::operator++();
			return *this;
		}
		
		FilteredChildIterator operator++( int )
		{
			FilteredChildIterator r( *this );
			BaseIterator::operator++();
			return r;
		}
		
};

} // namespace Gaffer

#endif GAFFER_FILTEREDCHILDITERATOR_H
