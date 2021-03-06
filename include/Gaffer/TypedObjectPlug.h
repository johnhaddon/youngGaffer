#ifndef GAFFER_TYPEDOBJECTPLUG_H
#define GAFFER_TYPEDOBJECTPLUG_H

#include "IECore/Object.h"

#include "Gaffer/ValuePlug.h"
#include "Gaffer/PlugIterator.h"

namespace Gaffer
{

/// A Plug type which can store values derived from IECore::Object.
/// \todo Should we remove the ability to specify a default value, since
/// we can't serialise all values? Or should we use some nasty MemIndexedIO
/// thing to support that?
template<typename T>
class TypedObjectPlug : public ValuePlug
{

	public :

		typedef T ValueType;
		typedef IECore::IntrusivePtr<T> ValuePtr;
		typedef IECore::IntrusivePtr<const T> ConstValuePtr;

		IECORE_RUNTIMETYPED_DECLARETEMPLATE( TypedObjectPlug<T>, ValuePlug );

		/// A copy of defaultValue is taken.
		TypedObjectPlug(
			const std::string &name = staticTypeName(),
			Direction direction=In,
			ConstValuePtr defaultValue = ValuePtr(),
			unsigned flags = None
		);
		virtual ~TypedObjectPlug();

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

		IE_CORE_DECLARERUNTIMETYPEDDESCRIPTION( TypedObjectPlug<T> );		

		void setValueInternal( ConstValuePtr value );
	
		ValuePtr m_value;
		ConstValuePtr m_defaultValue;

};

typedef TypedObjectPlug<IECore::Object> ObjectPlug;

IE_CORE_DECLAREPTR( ObjectPlug );

typedef FilteredChildIterator<PlugPredicate<Plug::Invalid, ObjectPlug> > ObjectPlugIterator;
typedef FilteredChildIterator<PlugPredicate<Plug::In, ObjectPlug> > InputObjectPlugIterator;
typedef FilteredChildIterator<PlugPredicate<Plug::Out, ObjectPlug> > OutputObjectPlugIterator;

} // namespace Gaffer

#endif GAFFER_TYPEDOBJECTPLUG_H
