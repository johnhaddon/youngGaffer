#ifndef GAFFER_TYPEDPLUG_H
#define GAFFER_TYPEDPLUG_H

#include "Gaffer/ValuePlug.h"

namespace Gaffer
{

template<typename T>
class TypedPlug : public ValuePlug
{

	public :

		typedef T ValueType;
		typedef boost::intrusive_ptr<TypedPlug> Ptr;
		typedef boost::intrusive_ptr<TypedPlug> ConstPtr;

		TypedPlug(
			const std::string &name = staticTypeName(),
			Direction direction=In,
			T defaultValue = T(),
			unsigned flags = None,
			PlugPtr input = 0
		);
		virtual ~TypedPlug();

		//! @name RunTimeTyped functions
		////////////////////////////////////
		//@{
		virtual IECore::TypeId typeId() const;
		virtual std::string typeName() const;
		virtual bool isInstanceOf( IECore::TypeId typeId ) const;
		virtual bool isInstanceOf( const std::string &typeName ) const;
		static IECore::TypeId staticTypeId();
		static std::string staticTypeName();
		static bool inheritsFrom( IECore::TypeId typeId );
		static bool inheritsFrom( const std::string &typeName );
		//@}

		/// Accepts only instances of TypedPlug<T> or derived classes.
		virtual bool acceptsInput( ConstPlugPtr input ) const;

		T defaultValue() const;

		/// \undoable
		void setValue( T value );
		/// Returns the value. This isn't const as it may require a compute
		/// and therefore a setValue().
		T getValue();

	protected :

		virtual void setFromInput();

	private :

		void setValueInternal( T value );
	
		T m_value;
		T m_defaultValue;

};

typedef TypedPlug<std::string> StringPlug;

IE_CORE_DECLAREPTR( StringPlug );

} // namespace Gaffer

#endif GAFFER_TYPEDPLUG_H
