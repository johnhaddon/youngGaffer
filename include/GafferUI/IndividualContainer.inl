#ifndef GAFFERUI_INDIVIDUALCONTAINER_INL
#define GAFFERUI_INDIVIDUALCONTAINER_INL

#include "GafferUI/IndividualContainer.h"

namespace GafferUI
{

template<typename T>
typename T::Ptr IndividualContainer::getChild()
{
	if( !children().size() )
	{
		return 0;
	}
	return Gaffer::GraphComponent::getChild<T>( (*(children().begin()))->getName() );
}

template<typename T>
typename T::ConstPtr IndividualContainer::getChild() const
{
	if( !children().size() )
	{
		return 0;
	}
	return IECore::runTimeCast<T>( *(children().begin()) );
}


} // namespace GafferUI

#endif // GAFFERUI_INDIVIDUALCONTAINER_INL
