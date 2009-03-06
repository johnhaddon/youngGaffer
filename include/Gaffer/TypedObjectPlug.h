#ifndef GAFFER_TYPEDOBJECTPLUG_H
#define GAFFER_TYPEDOBJECTPLUG_H

#include "IECore/Object.h"

#include "Gaffer/ValuePlug.h"

namespace Gaffer
{

/// A Plug type which can store value derived from IECore::Object.
template<typename T>
class TypedObjectPlug : public ValuePlug
{

	public :

		typedef T ValueType;
		typedef boost::intrusive_ptr<T> ValuePtr;
		typedef boost::intrusive_ptr<const T> ConstValuePtr;
		
		typedef boost::intrusive_ptr<TypedObjectPlug> Ptr;
		typedef boost::intrusive_ptr<TypedObjectPlug> ConstPtr;

		/// A copy of defaultValue is taken.
		TypedObjectPlug(
			const std::string &name = staticTypeName(),
			Direction direction=In,
			ConstValuePtr defaultValue = ValuePtr(),
			unsigned flags = None,
			PlugPtr input = 0
		);
		virtual ~TypedObjectPlug();

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

		/// Accepts only instances of TypedObjectPlug<T>, or derived classes.
		virtual bool acceptsInput( ConstPlugPtr input ) const;

		ConstValuePtr defaultValue() const;

		/// \undoable
		/// \todo This is taking a copy - does that cause terrible performance?
		void setValue( ConstValuePtr value );
		/// Returns the value. This isn't const as it may require a compute
		/// and therefore a setValue().
		ConstValuePtr getValue();

	protected :

		virtual void setFromInput();

	private :

		void setValueInternal( ConstValuePtr value );
	
		ValuePtr m_value;
		ConstValuePtr m_defaultValue;

};

typedef TypedObjectPlug<IECore::Object> ObjectPlug;

IE_CORE_DECLAREPTR( ObjectPlug );

} // namespace Gaffer

#endif GAFFER_TYPEDOBJECTPLUG_H