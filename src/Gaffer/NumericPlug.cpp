#include "Gaffer/NumericPlug.h"
#include "Gaffer/Action.h"

#include "OpenEXR/ImathFun.h"

#include "boost/bind.hpp"

using namespace Gaffer;

template<typename T>
const IECore::RunTimeTyped::TypeDescription<NumericPlug<T> > NumericPlug<T>::g_typeDescription;

namespace Gaffer
{

// RunTimeTyped specialisation
IECORE_RUNTIMETYPED_DEFINETEMPLATESPECIALISATION( IntPlug, IntPlugTypeId )
IECORE_RUNTIMETYPED_DEFINETEMPLATESPECIALISATION( FloatPlug, FloatPlugTypeId )

}

template<class T>
NumericPlug<T>::NumericPlug(
	const std::string &name,
	Direction direction,
	T defaultValue,
	T minValue,
	T maxValue,
	unsigned flags
)
	:	ValuePlug( name, direction, flags ),
		m_value( defaultValue ),
		m_defaultValue( defaultValue ),
		m_minValue( minValue ),
		m_maxValue( maxValue )
{
}

template<class T>
NumericPlug<T>::~NumericPlug()
{
}

template<class T>
bool NumericPlug<T>::acceptsInput( ConstPlugPtr input ) const
{
	if( !ValuePlug::acceptsInput( input ) )
	{
		return false;
	}
	return input->isInstanceOf( FloatPlug::staticTypeId() ) || input->isInstanceOf( IntPlug::staticTypeId() );
}

template<class T>
T NumericPlug<T>::defaultValue() const
{
	return m_defaultValue;
}

template<class T>
bool NumericPlug<T>::hasMinValue() const
{
	return m_minValue!=Imath::limits<T>::min();
}

template<class T>
bool NumericPlug<T>::hasMaxValue() const
{
	return m_maxValue!=Imath::limits<T>::max();
}

template<class T>
T NumericPlug<T>::minValue() const
{
	return m_minValue;
}

template<class T>
T NumericPlug<T>::maxValue() const
{
	return m_maxValue;
}
		
template<class T>
void NumericPlug<T>::setValue( T value )
{
	T v = Imath::clamp( value, m_minValue, m_maxValue );
	if( v!=m_value || getDirty() )
	{
		Action::enact(
			this,
			boost::bind( &NumericPlug<T>::setValueInternal, Ptr( this ), value ),
			boost::bind( &NumericPlug<T>::setValueInternal, Ptr( this ), m_value )		
		);
	}
}

template<class T>
void NumericPlug<T>::setValueInternal( T value )
{
	m_value = value;
	valueSet();	
}

template<class T>
T NumericPlug<T>::getValue()
{
	computeIfDirty();
	return m_value;
}

template<class T>
void NumericPlug<T>::setFromInput()
{
	PlugPtr i = getInput<Plug>();
	switch( i->typeId() )
	{
		case FloatPlugTypeId :
			setValue( boost::static_pointer_cast<FloatPlug>( i )->getValue() );
			break;
		case IntPlugTypeId :
			setValue( boost::static_pointer_cast<IntPlug>( i )->getValue() );
			break;
		default :
			assert( 0 ); // shouldn't have connections of any other type
	}
}

// explicit instantiation
template class NumericPlug<float>;
template class NumericPlug<int>;
