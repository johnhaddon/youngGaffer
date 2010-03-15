#include "Gaffer/TypedPlug.h"
#include "Gaffer/Action.h"

#include "OpenEXR/ImathFun.h"

#include "boost/bind.hpp"

using namespace Gaffer;

template<class T>
TypedPlug<T>::TypedPlug(
	const std::string &name,
	Direction direction,
	const T &defaultValue,
	unsigned flags
)
	:	ValuePlug( name, direction, flags ),
		m_value( defaultValue ),
		m_defaultValue( defaultValue )
{
}

template<class T>
TypedPlug<T>::~TypedPlug()
{
}

template<class T>
bool TypedPlug<T>::acceptsInput( ConstPlugPtr input ) const
{
	if( !ValuePlug::acceptsInput( input ) )
	{
		return false;
	}
	return input->isInstanceOf( staticTypeId() );
}

template<class T>
const T &TypedPlug<T>::defaultValue() const
{
	return m_defaultValue;
}
		
template<class T>
void TypedPlug<T>::setValue( const T &value )
{
	if( value!=m_value || getDirty() )
	{
		Action::enact(
			this,
			boost::bind( &TypedPlug<T>::setValueInternal, Ptr( this ), value ),
			boost::bind( &TypedPlug<T>::setValueInternal, Ptr( this ), m_value )		
		);
	}
}

template<class T>
void TypedPlug<T>::setValueInternal( T value )
{
	m_value = value;
	valueSet();
}

template<class T>
const T &TypedPlug<T>::getValue()
{
	computeIfDirty();
	return m_value;
}

template<class T>
void TypedPlug<T>::setFromInput()
{
	setValue( getInput<TypedPlug<T> >()->getValue() );
}

namespace Gaffer
{

IECORE_RUNTIMETYPED_DEFINETEMPLATESPECIALISATION( StringPlug, StringPlugTypeId )
IECORE_RUNTIMETYPED_DEFINETEMPLATESPECIALISATION( M33fPlug, M33fPlugTypeId )
IECORE_RUNTIMETYPED_DEFINETEMPLATESPECIALISATION( M44fPlug, M44fPlugTypeId )

}

// explicit instantiation
template class TypedPlug<std::string>;
template class TypedPlug<Imath::M33f>;
template class TypedPlug<Imath::M44f>;
