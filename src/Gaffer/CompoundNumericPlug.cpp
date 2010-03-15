#include "Gaffer/CompoundNumericPlug.h"

using namespace Gaffer;
using namespace boost;

template<typename T>
CompoundNumericPlug<T>::CompoundNumericPlug(
	const std::string &name,
	Direction direction,
	T defaultValue,
	T minValue,
	T maxValue,
	unsigned flags
)
	:	CompoundPlug( name, direction, flags )
{
	const char **n = childNames();
	for( unsigned i=0; i<T::dimensions(); i++ )
	{
		typename ChildType::Ptr p = new ChildType( *n++, direction, defaultValue[i], minValue[i], maxValue[i], flags );
		addChild( p );
	}
}

template<typename T>
CompoundNumericPlug<T>::~CompoundNumericPlug()
{
}

template<typename T>
bool CompoundNumericPlug<T>::acceptsChild( ConstGraphComponentPtr potentialChild ) const
{
	return children().size() != 3;
}

template<typename T>
typename CompoundNumericPlug<T>::ChildType::Ptr CompoundNumericPlug<T>::getChild( unsigned i )
{
	ChildContainer::const_iterator it = children().begin();
	while( i>0 )
	{
		it++;
		i -= 1;
	}
	return IECore::staticPointerCast<ChildType>( *it );
}

template<typename T>
typename CompoundNumericPlug<T>::ChildType::ConstPtr CompoundNumericPlug<T>::getChild( unsigned i ) const
{
	ChildContainer::const_iterator it = children().begin();
	while( i>0 )
	{
		it++;
		i -= 1;
	}
	return IECore::staticPointerCast<ChildType>( *it );
}

template<typename T>
T CompoundNumericPlug<T>::defaultValue() const
{
	T result;
	for( unsigned i=0; i<T::dimensions(); i++ )
	{
		result[i] = getChild( i )->defaultValue();
	}
	return result;
}

template<typename T>
bool CompoundNumericPlug<T>::hasMinValue() const
{
	for( unsigned i=0; i<T::dimensions(); i++ )
	{
		if( getChild( i )->hasMinValue() )
		{
			return true;
		}
	}
	return false;
}

template<typename T>
bool CompoundNumericPlug<T>::hasMaxValue() const
{
	for( unsigned i=0; i<T::dimensions(); i++ )
	{
		if( getChild( i )->hasMaxValue() )
		{
			return true;
		}
	}
	return false;
}

template<typename T>
T CompoundNumericPlug<T>::minValue() const
{
	T result;
	for( unsigned i=0; i<T::dimensions(); i++ )
	{
		result[i] = getChild( i )->minValue();
	}
	return result;
}

template<typename T>
T CompoundNumericPlug<T>::maxValue() const
{
	T result;
	for( unsigned i=0; i<T::dimensions(); i++ )
	{
		result[i] = getChild( i )->maxValue();
	}
	return result;
}

template<typename T>
void CompoundNumericPlug<T>::setValue( T value )
{
	for( unsigned i=0; i<T::dimensions(); i++ )
	{
		getChild( i )->setValue( value[i] );
	}
}

template<typename T>
T CompoundNumericPlug<T>::getValue()
{
	T result;
	for( unsigned i=0; i<T::dimensions(); i++ )
	{
		result[i] = getChild( i )->getValue();
	}
	return result;
}

template<typename T>
const char **CompoundNumericPlug<T>::childNames()
{
	static const char *names[] = { "x", "y", "z", "w" };
	return names;
}

// specialisations

namespace Gaffer
{

IECORE_RUNTIMETYPED_DEFINETEMPLATESPECIALISATION( V2fPlug, V2fPlugTypeId )
IECORE_RUNTIMETYPED_DEFINETEMPLATESPECIALISATION( V3fPlug, V3fPlugTypeId )
IECORE_RUNTIMETYPED_DEFINETEMPLATESPECIALISATION( V2iPlug, V2iPlugTypeId )
IECORE_RUNTIMETYPED_DEFINETEMPLATESPECIALISATION( V3iPlug, V3iPlugTypeId )
IECORE_RUNTIMETYPED_DEFINETEMPLATESPECIALISATION( Color3fPlug, Color3fPlugTypeId )
IECORE_RUNTIMETYPED_DEFINETEMPLATESPECIALISATION( Color4fPlug, Color4fPlugTypeId )

}

// explicit instantiations

template class CompoundNumericPlug<Imath::V2f>;
template class CompoundNumericPlug<Imath::V3f>;
template class CompoundNumericPlug<Imath::V2i>;
template class CompoundNumericPlug<Imath::V3i>;
template class CompoundNumericPlug<Imath::Color3f>;
template class CompoundNumericPlug<Imath::Color4f>;
