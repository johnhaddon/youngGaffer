#ifndef GAFFER_PLUGITERATOR_H
#define GAFFER_PLUGITERATOR_H

#include "Gaffer/Plug.h"
#include "Gaffer/FilteredChildIterator.h"

#include "boost/iterator/filter_iterator.hpp"

namespace Gaffer
{

template<Plug::Direction D=Plug::Invalid, typename T=Plug>
struct PlugPredicate
{
	typedef T ChildType;

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

typedef FilteredChildIterator<PlugPredicate<> > PlugIterator;
typedef FilteredChildIterator<PlugPredicate<Plug::In, Plug> > InputPlugIterator;
typedef FilteredChildIterator<PlugPredicate<Plug::Out, Plug> > OutputPlugIterator;

} // namespace Gaffer

#endif GAFFER_PLUGITERATOR_H
