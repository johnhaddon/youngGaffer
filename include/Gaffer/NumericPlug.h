#ifndef GAFFER_NUMERICPLUG_H
#define GAFFER_NUMERICPLUG_H

#include "Gaffer/ValuePlug.h"

#include "OpenEXR/ImathLimits.h"

namespace Gaffer
{

template<typename T>
class NumericPlug : public ValuePlug
{

	public :

		typedef T ValueType;
		typedef boost::intrusive_ptr<NumericPlug> Ptr;
		typedef boost::intrusive_ptr<NumericPlug> ConstPtr;

		NumericPlug(
			const std::string &name = staticTypeName(),
			Direction direction=In,
			T defaultValue = T(),
			T minValue = Imath::limits<T>::min(),
			T maxValue = Imath::limits<T>::max()
		);
		virtual ~NumericPlug();

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

		/// Accepts other NumericPlugs, including those of different types.
		virtual bool acceptsInput( ConstPlugPtr input ) const;

		T defaultValue() const;
		T minValue() const;
		T maxValue() const;

		/// Clamps the value between min and max.
		void setValue( T value );
		/// Returns the value. This isn't const as it may require a compute
		/// and therefore a setValue().
		T getValue();

	protected :

		virtual void setFromInput();

	private :
	
		T m_value;
		T m_defaultValue;
		T m_minValue;
		T m_maxValue;

};

typedef NumericPlug<float> FloatPlug;
typedef NumericPlug<int> IntPlug;

IE_CORE_DECLAREPTR( FloatPlug );
IE_CORE_DECLAREPTR( IntPlug );

} // namespace Gaffer

#endif // GAFFER_NUMERICPLUG_H
