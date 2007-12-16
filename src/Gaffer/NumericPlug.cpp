#include "Gaffer/NumericPlug.h"

#include "OpenEXR/ImathFun.h"

using namespace Gaffer;

#define SPECIALISE( TNAME )															\
																					\
	template<>																		\
	IECore::TypeId TNAME::staticTypeId()											\
	{																				\
		return (IECore::TypeId)TNAME ## TypeId;										\
	}																				\
																					\
	template<>																		\
	std::string TNAME::staticTypeName()												\
	{																				\
		return # TNAME;																\
	}																				\
																					\

template<class T>
IECore::TypeId NumericPlug<T>::typeId() const
{
	return staticTypeId();
}

template<class T>
std::string NumericPlug<T>::typeName() const
{
	return staticTypeName();
}

template<class T>
bool NumericPlug<T>::isInstanceOf( IECore::TypeId typeId ) const
{
	if( typeId==staticTypeId() )
	{
		return true;
	}
	return ValuePlug::isInstanceOf( typeId );
}


template<class T>
bool NumericPlug<T>::isInstanceOf( const std::string &typeName ) const
{
	if( typeName==staticTypeName() )
	{
		return true;
	}
	return ValuePlug::isInstanceOf( typeName );
}


template<class T>
bool NumericPlug<T>::inheritsFrom( IECore::TypeId typeId )
{
	return ValuePlug::staticTypeId()==typeId ? true : ValuePlug::inheritsFrom( typeId );
}


template<class T>
bool NumericPlug<T>::inheritsFrom( const std::string &typeName )
{
	return ValuePlug::staticTypeName()==typeName ? true : ValuePlug::inheritsFrom( typeName );
}

template<class T>
NumericPlug<T>::NumericPlug(
	const std::string &name,
	Direction direction,
	T defaultValue,
	T minValue,
	T maxValue
)
	:	ValuePlug( name, direction ),
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
	if( v!=m_value )
	{
		m_value = v;
		valueSet();
	}
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

SPECIALISE( IntPlug )
SPECIALISE( FloatPlug )

// explicit instantiation
template class NumericPlug<float>;
template class NumericPlug<int>;
