#ifndef GAFFERUI_EVENTSIGNALCOMBINER_INL
#define GAFFERUI_EVENTSIGNALCOMBINER_INL

namespace GafferUI
{

template<typename T>
template<typename InputIterator>
typename EventSignalCombiner<T>::result_type EventSignalCombiner<T>::operator()(InputIterator first, InputIterator last) const
{
	while( first != last )
	{
		result_type r = *first;
		if( r )
		{
			return r;
		}
		first++;
	}
	return 0;
};

} // namespace GafferUI

#endif // GAFFERUI_EVENTSIGNALCOMBINER_INL
