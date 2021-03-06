#include "Gaffer/TypedObjectPlug.h"
#include "Gaffer/Action.h"

#include "OpenEXR/ImathFun.h"

#include "boost/bind.hpp"

using namespace Gaffer;

template<class T>
TypedObjectPlug<T>::TypedObjectPlug(
	const std::string &name,
	Direction direction,
	ConstValuePtr defaultValue,
	unsigned flags
)
	:	ValuePlug( name, direction, flags ),
		m_value( defaultValue ? defaultValue->copy() : 0 ),
		m_defaultValue( defaultValue ? defaultValue->copy() : 0 )
{
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

namespace Gaffer
{

IECORE_RUNTIMETYPED_DEFINETEMPLATESPECIALISATION( ObjectPlug, ObjectPlugTypeId )

}

// explicit instantiation
template class TypedObjectPlug<IECore::Object>;
