#ifndef GAFFER_PLUGITERATOR_H
#define GAFFER_PLUGITERATOR_H

#include "Gaffer/Plug.h"

#include "boost/iterator/filter_iterator.hpp"

namespace Gaffer
{

template<Plug::Direction D=Plug::Invalid, typename T=Plug>
struct PlugPredicate
{
	bool operator()( GraphComponentPtr g )
	{
		typename T::Ptr p = IECore::runTimeCast<T>( g );
		if( !p )
		{
			return false;
		}
		if( D==Plug::Invalid )
		{
			return true;
		}
		return D==p->direction();
	}
};

typedef PlugPredicate<Plug::In> InputPlugPredicate;
typedef PlugPredicate<Plug::Out> OutputPlugPredicate;

typedef boost::filter_iterator<PlugPredicate<>, GraphComponent::ChildIterator> PlugIterator;
typedef boost::filter_iterator<InputPlugPredicate, GraphComponent::ChildIterator> InputPlugIterator;
typedef boost::filter_iterator<OutputPlugPredicate, GraphComponent::ChildIterator> OutputPlugIterator;

} // namespace Gaffer

#endif GAFFER_PLUGITERATOR_H
