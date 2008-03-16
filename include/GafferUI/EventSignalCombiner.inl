#ifndef GAFFERUI_EVENTSIGNALCOMBINER_INL
#define GAFFERUI_EVENTSIGNALCOMBINER_INL

namespace GafferUI
{

template<typename InputIterator>
EventSignalCombiner::result_type EventSignalCombiner::operator()(InputIterator first, InputIterator last) const
{
	while( first != last )
	{
		if( *first )
		{
			return true;
		}
		first++;
	}
	return false;
};

} // namespace GafferUI

#endif // GAFFERUI_EVENTSIGNALCOMBINER_INL
