#ifndef GAFFER_COMPOUNDNUMERICPLUG_H
#define GAFFER_COMPOUNDNUMERICPLUG_H

#include "OpenEXR/ImathVec.h"

#include "Gaffer/CompoundPlug.h"
#include "Gaffer/NumericPlug.h"

namespace Gaffer
{

template<typename T>
class CompoundNumericPlug : public CompoundPlug
{

	public :

		typedef T ValueType;
		typedef NumericPlug<typename T::BaseType> ChildType;
		
		IECORE_RUNTIMETYPED_DECLARETEMPLATE( CompoundNumericPlug<T>, CompoundPlug );

		CompoundNumericPlug(
			const std::string &name = staticTypeName(),
			Direction direction=In,
			T defaultValue = T( 0 ),
			T minValue = T( Imath::limits<T>::min() ),
			T maxValue = T( Imath::limits<T>::max() ),
			unsigned flags = None
		);
		virtual ~CompoundNumericPlug();

		/// Accepts no children following construction.
		virtual bool acceptsChild( ConstGraphComponentPtr potentialChild ) const;

		typename ChildType::Ptr getChild( unsigned i );
		typename ChildType::ConstPtr getChild( unsigned i ) const;	

		T defaultValue() const;
		
		bool hasMinValue() const;
		bool hasMaxValue() const;

		T minValue() const;
		T maxValue() const;
		
		/// Calls setValue for each of the child plugs, passing the components
		/// of value.
		/// \undoable
		void setValue( T value );
		/// Returns the value, calling getValue() on each child plug to compute a component
		/// of the result. This isn't const as it may require a compute and therefore a setValue().
		T getValue();
		
	private :
	
		static const char **childNames();
	
};

typedef CompoundNumericPlug<Imath::V2f> V2fPlug;
typedef CompoundNumericPlug<Imath::V3f> V3fPlug;

typedef CompoundNumericPlug<Imath::V2i> V2iPlug;
typedef CompoundNumericPlug<Imath::V3i> V3iPlug;

IE_CORE_DECLAREPTR( V2fPlug );
IE_CORE_DECLAREPTR( V3fPlug );
IE_CORE_DECLAREPTR( V2iPlug );
IE_CORE_DECLAREPTR( V3iPlug );

} // namespace Gaffer

#endif // GAFFER_COMPOUNDNUMERICPLUG_H
