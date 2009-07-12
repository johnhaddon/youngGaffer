#include "Gaffer/SplinePlug.h"

#include "boost/format.hpp"

using namespace Gaffer;

template<typename T>
const IECore::RunTimeTyped::TypeDescription<SplinePlug<T> > SplinePlug<T>::g_typeDescription;

template<typename T>
SplinePlug<T>::SplinePlug( const std::string &name, Direction direction, const T &defaultValue, unsigned flags )
	:	CompoundPlug( name, direction, flags ), m_defaultValue( defaultValue )
{
	CompoundPlugPtr basis = new CompoundPlug( "basis" );
	M44fPlugPtr basisMatrix = new M44fPlug( "matrix" );
	IntPlugPtr basisStep = new IntPlug( "step" );
	basis->addChild( basisMatrix );
	basis->addChild( basisStep );
	addChild( basis );

	setValue( defaultValue );
}

template<typename T>
SplinePlug<T>::~SplinePlug()
{
}

template<typename T>
bool SplinePlug<T>::acceptsChild( ConstGraphComponentPtr potentialChild ) const
{
	if( !children().size() )
	{
		// to let the basis plug through during construction
		return true;
	}

	ConstCompoundPlugPtr c = IECore::runTimeCast<const CompoundPlug>( potentialChild );
	if( !c )
	{
		return false;
	}
	if( c->children().size()!=2 )
	{
		return false;
	}
	if( !c->getChild<XPlugType>( "x" ) )
	{
		return false;
	}
	if( !c->getChild<YPlugType>( "y" ) )
	{
		return false;
	}
	return true;
}

template<typename T>
const T &SplinePlug<T>::defaultValue() const
{
	return m_defaultValue;
}

template<typename T>
void SplinePlug<T>::setValue( const T &value )
{
	basisMatrixPlug()->setValue( value.basis.matrix );
	basisStepPlug()->setValue( value.basis.step );
	
	unsigned existingPoints = numPoints();
	unsigned i = 0;
	for( typename T::PointContainer::const_iterator it=value.points.begin(); it!=value.points.end(); it++ )
	{
		if( i >= existingPoints )
		{
			addPoint();
		}
		pointXPlug( i )->setValue( it->first );
		pointYPlug( i )->setValue( it->second );
		i++;
	}
	
	while( i < existingPoints )
	{
		removeChild( pointPlug( i++ ) );
	}
}

template<typename T>
T SplinePlug<T>::getValue()
{
	T result;
	result.basis.matrix = basisMatrixPlug()->getValue();
	result.basis.step = basisStepPlug()->getValue();
	
	unsigned n = numPoints();
	for( unsigned i=0; i<n; i++ )
	{
		result.points.insert( typename T::PointContainer::value_type( pointXPlug( i )->getValue(), pointYPlug( i )->getValue() ) );
	}
	
	return result;
}

template<typename T>
CompoundPlugPtr SplinePlug<T>::basisPlug()
{
	return getChild<CompoundPlug>( "basis" );
}

template<typename T>
ConstCompoundPlugPtr SplinePlug<T>::basisPlug() const
{	
	return getChild<CompoundPlug>( "basis" );
}

template<typename T>
M44fPlugPtr SplinePlug<T>::basisMatrixPlug()
{
	return basisPlug()->getChild<M44fPlug>( "matrix" );
}

template<typename T>
ConstM44fPlugPtr SplinePlug<T>::basisMatrixPlug() const
{
	return basisPlug()->getChild<M44fPlug>( "matrix" );
}

template<typename T>
IntPlugPtr SplinePlug<T>::basisStepPlug()
{
	return basisPlug()->getChild<IntPlug>( "step" );
}

template<typename T>
ConstIntPlugPtr SplinePlug<T>::basisStepPlug() const
{
	return basisPlug()->getChild<IntPlug>( "step" );
}

template<typename T>
unsigned SplinePlug<T>::numPoints() const
{
	return children().size() - 1;
}

template<typename T>
unsigned SplinePlug<T>::addPoint()
{
	unsigned n = numPoints();
	CompoundPlugPtr p = new CompoundPlug( pointPlugName( n ), direction() );
	typename XPlugType::Ptr x = new XPlugType( "x", direction(), typename T::XType( 0 ) );
	x->setFlags( Plug::Dynamic );
	p->addChild( x );
	
	typename YPlugType::Ptr y = new YPlugType( "y", direction(), typename T::YType( 0 ) );
	y->setFlags( Plug::Dynamic );
	p->addChild( y );
	
	addChild( p );
	
	return n + 1;
}

template<typename T>
void SplinePlug<T>::removePoint( unsigned pointIndex )
{
	removeChild( pointPlug( pointIndex ) );
}

template<typename T>
std::string SplinePlug<T>::pointPlugName( unsigned pointIndex ) const
{
	static boost::format formatter( "p%1%" );
	return boost::str( formatter % pointIndex );
}

template<typename T>
CompoundPlugPtr SplinePlug<T>::pointPlug( unsigned pointIndex )
{
	return getChild<CompoundPlug>( pointPlugName( pointIndex ) );
}

template<typename T>
ConstCompoundPlugPtr SplinePlug<T>::pointPlug( unsigned pointIndex ) const
{
	return getChild<CompoundPlug>( pointPlugName( pointIndex ) );
}

template<typename T>
typename SplinePlug<T>::XPlugType::Ptr SplinePlug<T>::pointXPlug( unsigned pointIndex )
{
	return pointPlug( pointIndex )->getChild<XPlugType>( "x" );
}

template<typename T>
typename SplinePlug<T>::XPlugType::ConstPtr SplinePlug<T>::pointXPlug( unsigned pointIndex ) const
{
	return pointPlug( pointIndex )->getChild<XPlugType>( "x" );
}

template<typename T>
typename SplinePlug<T>::YPlugType::Ptr SplinePlug<T>::pointYPlug( unsigned pointIndex )
{
	return pointPlug( pointIndex )->getChild<YPlugType>( "y" );
}

template<typename T>
typename SplinePlug<T>::YPlugType::ConstPtr SplinePlug<T>::pointYPlug( unsigned pointIndex ) const
{
	return pointPlug( pointIndex )->getChild<YPlugType>( "y" );
}

// RunTimeTyped specialisation
IECORE_RUNTIMETYPED_DEFINETEMPLATESPECIALISATION( SplineffPlug, SplineffPlugTypeId )
IECORE_RUNTIMETYPED_DEFINETEMPLATESPECIALISATION( SplinefColor3fPlug, SplinefColor3fPlugTypeId )

// explicit instantiation
template class SplinePlug<IECore::Splineff>;
template class SplinePlug<IECore::SplinefColor3f>;
