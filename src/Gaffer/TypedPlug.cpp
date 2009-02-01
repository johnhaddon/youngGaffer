#include "Gaffer/TypedPlug.h"
#include "Gaffer/Action.h"

#include "OpenEXR/ImathFun.h"

#include "boost/bind.hpp"

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
IECore::TypeId TypedPlug<T>::typeId() const
{
	return staticTypeId();
}

template<class T>
std::string TypedPlug<T>::typeName() const
{
	return staticTypeName();
}

template<class T>
bool TypedPlug<T>::isInstanceOf( IECore::TypeId typeId ) const
{
	if( typeId==staticTypeId() )
	{
		return true;
	}
	return ValuePlug::isInstanceOf( typeId );
}


template<class T>
bool TypedPlug<T>::isInstanceOf( const std::string &typeName ) const
{
	if( typeName==staticTypeName() )
	{
		return true;
	}
	return ValuePlug::isInstanceOf( typeName );
}


template<class T>
bool TypedPlug<T>::inheritsFrom( IECore::TypeId typeId )
{
	return ValuePlug::staticTypeId()==typeId ? true : ValuePlug::inheritsFrom( typeId );
}


template<class T>
bool TypedPlug<T>::inheritsFrom( const std::string &typeName )
{
	return ValuePlug::staticTypeName()==typeName ? true : ValuePlug::inheritsFrom( typeName );
}

template<class T>
TypedPlug<T>::TypedPlug(
	const std::string &name,
	Direction direction,
	T defaultValue,
	unsigned flags,
	PlugPtr input
)
	:	ValuePlug( name, direction, flags, 0 ),
		m_value( defaultValue ),
		m_defaultValue( defaultValue )
{
	setInput( input );
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
T TypedPlug<T>::defaultValue() const
{
	return m_defaultValue;
}
		
template<class T>
void TypedPlug<T>::setValue( T value )
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
T TypedPlug<T>::getValue()
{
	computeIfDirty();
	return m_value;
}

template<class T>
void TypedPlug<T>::setFromInput()
{
	setValue( getInput<TypedPlug<T> >()->getValue() );
}

SPECIALISE( StringPlug )

// explicit instantiation
template class TypedPlug<std::string>;
