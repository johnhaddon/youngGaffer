#ifndef GAFFER_TYPEDPLUG_H
#define GAFFER_TYPEDPLUG_H

#include "OpenEXR/ImathMatrix.h"

#include "Gaffer/ValuePlug.h"

namespace Gaffer
{

template<typename T>
class TypedPlug : public ValuePlug
{

	public :

		typedef T ValueType;

		IECORE_RUNTIMETYPED_DECLARETEMPLATE( TypedPlug<T>, ValuePlug );

		TypedPlug(
			const std::string &name = staticTypeName(),
			Direction direction=In,
			const T &defaultValue = T(),
			unsigned flags = None
		);
		virtual ~TypedPlug();

		/// Accepts only instances of TypedPlug<T> or derived classes.
		virtual bool acceptsInput( ConstPlugPtr input ) const;

		const T &defaultValue() const;

		/// \undoable
		void setValue( const T &value );
		/// Returns the value. This isn't const as it may require a compute
		/// and therefore a setValue().
		const T &getValue();

	protected :

		virtual void setFromInput();

	private :

		IE_CORE_DECLARERUNTIMETYPEDDESCRIPTION( TypedPlug<T> );		

		void setValueInternal( T value );
	
		T m_value;
		T m_defaultValue;

};

typedef TypedPlug<std::string> StringPlug;
typedef TypedPlug<Imath::M33f> M33fPlug;
typedef TypedPlug<Imath::M44f> M44fPlug;

IE_CORE_DECLAREPTR( StringPlug );
IE_CORE_DECLAREPTR( M33fPlug );
IE_CORE_DECLAREPTR( M44fPlug );

} // namespace Gaffer

#endif // GAFFER_TYPEDPLUG_H
