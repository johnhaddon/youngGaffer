#include "GafferUI/Frame.h"

#include "IECore/CurvesPrimitive.h"
#include "IECore/MeshPrimitive.h"
#include "IECore/SimpleTypedData.h"

#include "math.h"

using namespace GafferUI;
using namespace IECore;
using namespace Imath;

static const int cornerSegments = 10;

/// \todo I suspect there should be a central resource class providing these shapes to all
/// Gadgets.
static CurvesPrimitivePtr cornerCurve()
{
	static CurvesPrimitivePtr c = 0;
	if( !c )
	{
		IntVectorDataPtr vertsPerCurve = new IntVectorData;
		vertsPerCurve->writable().push_back( cornerSegments );
		
		V3fVectorDataPtr points = new V3fVectorData;
		for( int i=0; i<cornerSegments; i++ )
		{
			float t = (float)i/(float)(cornerSegments - 1 );
			t *= M_PI / 2.0f;
			points->writable().push_back( V3f( cos( t ), sin( t ), 0 ) );
			
		}
		c = new CurvesPrimitive( vertsPerCurve, CubicBasisf::linear(), false, points );
	}
	return c;
}

static MeshPrimitivePtr cornerMesh()
{
	static MeshPrimitivePtr m = 0;
	if( !m )
	{
		IntVectorDataPtr vertsPerFace = new IntVectorData;
		vertsPerFace->writable().push_back( cornerSegments + 1 );
		
		V3fVectorDataPtr points = new V3fVectorData;
		IntVectorDataPtr vertIds = new IntVectorData;
		for( int i=0; i<cornerSegments; i++ )
		{
			float t = (float)i/(float)(cornerSegments - 1 );
			t *= M_PI / 2.0f;
			points->writable().push_back( V3f( cos( t ), sin( t ), 0 ) );
			vertIds->writable().push_back( i );
		}
		points->writable().push_back( V3f( 0 ) );
		vertIds->writable().push_back( cornerSegments );
		
		m = new MeshPrimitive( vertsPerFace, vertIds, "linear", points );
	}
	return m;
}

Frame::Frame( GadgetPtr child )
	:	IndividualContainer( child ), m_border( 1 )
{
}

Frame::~Frame()
{
}

Imath::Box3f Frame::bound() const
{
	Imath::Box3f b = IndividualContainer::bound();
	if( b.isEmpty() )
	{
		return b;
	}
	b.max += V3f( m_border, m_border, 0 );
	b.min -= V3f( m_border, m_border, 0 );
	return b;
}

void Frame::doRender( IECore::RendererPtr renderer ) const
{
	Imath::Box3f b = IndividualContainer::bound();
	
	renderer->attributeBegin();
	
		renderer->setAttribute( "gl:curvesPrimitive:useGLLines", new IECore::BoolData( true ) );
	
		M44f m;
		renderer->transformBegin();
			m.setTranslation( V3f( b.max.x, b.max.y, 0 ) );
			renderer->concatTransform( m );
			cornerCurve()->render( renderer );
			cornerMesh()->render( renderer );
		renderer->transformEnd();
		
		renderer->transformBegin();
			m.setTranslation( V3f( b.max.x, b.min.y, 0 ) );
			m.scale( V3f( m_border, -m_border, 0 ) );
			renderer->concatTransform( m );
			cornerCurve()->render( renderer );
			cornerMesh()->render( renderer );
		renderer->transformEnd();
		
		renderer->transformBegin();
			m.setTranslation( V3f( b.min.x, b.min.y, 0 ) );
			m.scale( V3f( -m_border, -m_border, 0 ) );
			renderer->concatTransform( m );
			cornerCurve()->render( renderer );
			cornerMesh()->render( renderer );
		renderer->transformEnd();
		
		renderer->transformBegin();
			m.setTranslation( V3f( b.min.x, b.max.y, 0 ) );
			m.scale( V3f( -m_border, m_border, 0 ) );
			renderer->concatTransform( m );
			cornerCurve()->render( renderer );
			cornerMesh()->render( renderer );
		renderer->transformEnd();
	
	renderer->attributeEnd();
	
	IndividualContainer::doRender( renderer );
}
