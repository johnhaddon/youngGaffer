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
	bool operator()( IECore::RunTimeTypedPtr x )
	{
		return IECore::runTimeCast<T>( x );
	}
};

template<typename T>
class FilteredChildIterator : public boost::filter_iterator<TypePredicate<T>, GraphComponent::ChildIterator>
{
	
	public :
	
		typedef boost::filter_iterator<TypePredicate<T>, GraphComponent::ChildIterator> BaseIterator;
		
		typedef boost::intrusive_ptr<T> reference;
		
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
			// static cast is safe as predicate has checked type
			return boost::static_pointer_cast<T>( BaseIterator::operator*() );
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
