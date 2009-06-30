#ifndef GAFFER_SPLINEPLUG_H
#define GAFFER_SPLINEPLUG_H

#include "IECore/Spline.h"

#include "Gaffer/CompoundPlug.h"
#include "Gaffer/NumericPlug.h"
#include "Gaffer/TypedPlug.h"
#include "Gaffer/PlugType.h"

namespace Gaffer
{

template<typename T>
class SplinePlug : public CompoundPlug
{

	public :

		typedef T ValueType;
		typedef typename PlugType<typename T::XType>::Type XPlugType;
		typedef typename PlugType<typename T::YType>::Type YPlugType;
		
		IECORE_RUNTIMETYPED_DECLARETEMPLATE( SplinePlug<T>, CompoundPlug );

		SplinePlug(
			const std::string &name = staticTypeName(),
			Direction direction=In,
			const T &defaultValue = T(),
			unsigned flags = None
		);
		virtual ~SplinePlug();

		/// Implemented to only accept children which are suitable for use as points
		/// in the spline.
		virtual bool acceptsChild( ConstGraphComponentPtr potentialChild ) const;

		const T &defaultValue() const;
		
		/// \undoable
		void setValue( const T &value );
		T getValue();
		
		CompoundPlugPtr basisPlug();
		ConstCompoundPlugPtr basisPlug() const;
		M44fPlugPtr basisMatrixPlug();
		ConstM44fPlugPtr basisMatrixPlug() const;
		IntPlugPtr basisStepPlug();
		ConstIntPlugPtr basisStepPlug() const;
		
		unsigned numPoints() const;
		unsigned addPoint();
		void removePoint( unsigned pointIndex );
		std::string pointPlugName( unsigned pointIndex ) const;
		CompoundPlugPtr pointPlug( unsigned pointIndex );
		ConstCompoundPlugPtr pointPlug( unsigned pointIndex ) const;
		typename XPlugType::Ptr pointXPlug( unsigned pointIndex );
		typename XPlugType::ConstPtr pointXPlug( unsigned pointIndex ) const;
		typename YPlugType::Ptr pointYPlug( unsigned pointIndex );
		typename YPlugType::ConstPtr pointYPlug( unsigned pointIndex ) const;

	private :

		T m_defaultValue;

};

typedef SplinePlug<IECore::Splineff> SplineffPlug;
typedef SplinePlug<IECore::SplinefColor3f> SplinefColor3fPlug;

IE_CORE_DECLAREPTR( SplineffPlug );
IE_CORE_DECLAREPTR( SplinefColor3fPlug );

} // namespace Gaffer

#endif // GAFFER_SPLINEPLUG_H
