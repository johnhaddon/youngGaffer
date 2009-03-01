#include "Gaffer/TypedObjectPlug.h"
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
IECore::TypeId TypedObjectPlug<T>::typeId() const
{
	return staticTypeId();
}

template<class T>
std::string TypedObjectPlug<T>::typeName() const
{
	return staticTypeName();
}

template<class T>
bool TypedObjectPlug<T>::isInstanceOf( IECore::TypeId typeId ) const
{
	if( typeId==staticTypeId() )
	{
		return true;
	}
	return ValuePlug::isInstanceOf( typeId );
}


template<class T>
bool TypedObjectPlug<T>::isInstanceOf( const std::string &typeName ) const
{
	if( typeName==staticTypeName() )
	{
		return true;
	}
	return ValuePlug::isInstanceOf( typeName );
}


template<class T>
bool TypedObjectPlug<T>::inheritsFrom( IECore::TypeId typeId )
{
	return ValuePlug::staticTypeId()==typeId ? true : ValuePlug::inheritsFrom( typeId );
}


template<class T>
bool TypedObjectPlug<T>::inheritsFrom( const std::string &typeName )
{
	return ValuePlug::staticTypeName()==typeName ? true : ValuePlug::inheritsFrom( typeName );
}

template<class T>
TypedObjectPlug<T>::TypedObjectPlug(
	const std::string &name,
	Direction direction,
	ConstValuePtr defaultValue,
	unsigned flags,
	PlugPtr input
)
	:	ValuePlug( name, direction, flags, 0 ),
		m_value( defaultValue ? defaultValue->copy() : 0 ),
		m_defaultValue( defaultValue ? defaultValue->copy() : 0 )
{
	setInput( input );
}

template<class T>
TypedObjectPlug<T>::~TypedObjectPlug()
{
}

template<class T>
bool TypedObjectPlug<T>::acceptsInput( ConstPlugPtr input ) const
{
	if( !ValuePlug::acceptsInput( input ) )
	{
		return false;
	}
	return input->isInstanceOf( staticTypeId() );
}

template<class T>
typename TypedObjectPlug<T>::ConstValuePtr TypedObjectPlug<T>::defaultValue() const
{
	return m_defaultValue;
}
		
template<class T>
void TypedObjectPlug<T>::setValue( ConstValuePtr value )
{
	// the other plug types only actually do anything if the new value is different than the old.
	// we don't do that here as it's assumed that the cost of testing large objects for equality
	// is greater than the cost of the additional callbacks invoked if the value is set to the same
	// again.
	Action::enact(
		this,
		boost::bind( &TypedObjectPlug<T>::setValueInternal, Ptr( this ), value ),
		boost::bind( &TypedObjectPlug<T>::setValueInternal, Ptr( this ), m_value )		
	);
}

template<class T>
void TypedObjectPlug<T>::setValueInternal( ConstValuePtr value )
{
	if( value )
	{
		m_value = value->copy();
	}
	else
	{
		m_value = 0;
	}
	valueSet();
}

template<class T>
typename TypedObjectPlug<T>::ConstValuePtr TypedObjectPlug<T>::getValue()
{
	computeIfDirty();
	return m_value;
}

template<class T>
void TypedObjectPlug<T>::setFromInput()
{
	setValue( getInput<TypedObjectPlug<T> >()->getValue() );
}

SPECIALISE( ObjectPlug )

// explicit instantiation
template class TypedObjectPlug<IECore::Object>;
