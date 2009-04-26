#include "GafferUI/Frame.h"
#include "GafferUI/Style.h"

#include "IECore/CurvesPrimitive.h"
#include "IECore/MeshPrimitive.h"
#include "IECore/SimpleTypedData.h"

#include "math.h"

using namespace GafferUI;
using namespace IECore;
using namespace Imath;
using namespace boost;

IE_CORE_DEFINERUNTIMETYPED( Frame );

static const int cornerSegments = 10;

/// \todo I suspect there should be a central resource class providing these shapes to all
/// Gadgets. Maybe it's a Style class? And it has methods to draw these elements straight
/// to a renderer?
/*static CurvesPrimitivePtr cornerCurve()
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
}*/

/*static CurvesPrimitivePtr horizonalLine()
{
	static CurvesPrimitivePtr c = 0;
	if( !c )
	{
s		IntVectorDataPtr vertsPerCurve = new IntVectorData;
		vertsPerCurve->writable().push_back( 2 );
		
		V3fVectorDataPtr points = new V3fVectorData;
		points->writable().push_back( V3f( 0, 0, 0  ) );
		points->writable().push_back( V3f( 1, 0, 0  ) );
		c = new CurvesPrimitive( vertsPerCurve, CubicBasisf::linear(), false, points );
	}
	return c;
}

static CurvesPrimitivePtr verticalLine()
{
	static CurvesPrimitivePtr c = 0;
	if( !c )
	{
		IntVectorDataPtr vertsPerCurve = new IntVectorData;
		vertsPerCurve->writable().push_back( 2 );
		
		V3fVectorDataPtr points = new V3fVectorData;
		points->writable().push_back( V3f( 0, 0, 0  ) );
		points->writable().push_back( V3f( 0, 1, 0  ) );
		c = new CurvesPrimitive( vertsPerCurve, CubicBasisf::linear(), false, points );
	}
	return c;
}*/
/*
static MeshPrimitivePtr rectangleMesh()
{
	static MeshPrimitivePtr m = 0;
	if( !m )
	{
		IntVectorDataPtr vertsPerFace = new IntVectorData;
		vertsPerFace->writable().push_back( 4 );
		
		V3fVectorDataPtr points = new V3fVectorData;
		IntVectorDataPtr vertIds = new IntVectorData;
		
		points->writable().push_back( V3f( 0, 0, 0 ) );
		points->writable().push_back( V3f( 1, 0, 0 ) );
		points->writable().push_back( V3f( 1, 1, 0 ) );
		points->writable().push_back( V3f( 0, 1, 0 ) );
		
		vertIds->writable().push_back( 0 );
		vertIds->writable().push_back( 1 );
		vertIds->writable().push_back( 2 );
		vertIds->writable().push_back( 3 );
		
		m = new MeshPrimitive( vertsPerFace, vertIds, "linear", points );
	}
	return m;
}*/

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
	
		/// \todo These shouldn't be here
		renderer->setAttribute( "gl:curvesPrimitive:useGLLines", new IECore::BoolData( true ) );
		//renderer->setAttribute( "color", new IECore::Color3fData( Color3f( 0.2 ) ) );
	
		getStyle()->renderFrame( renderer, Box2f( V2f( b.min.x, b.min.y ), V2f( b.max.x, b.max.y ) ), m_border );
	
		/// \todo Remove All this dead code. It's just here in case it needs moving into the Style class at some point.
		/*M44f m;
		MeshPrimitivePtr rectangle = rectangleMesh();
		V3fVectorDataPtr p = static_pointer_cast<V3fVectorData>( rectangle->variables["P"].data );
		p->writable()[0] = V3f( b.min.x, b.min.y - m_border, 0 );
		p->writable()[1] = V3f( b.max.x, b.min.y - m_border, 0 );
		p->writable()[2] = V3f( b.max.x, b.max.y + m_border, 0 );
		p->writable()[3] = V3f( b.min.x, b.max.y + m_border, 0 );
		rectangle->render( renderer );
		
		p->writable()[0] = V3f( b.min.x - m_border, b.min.y, 0 );
		p->writable()[1] = V3f( b.min.x, b.min.y, 0 );
		p->writable()[2] = V3f( b.min.x, b.max.y, 0 );
		p->writable()[3] = V3f( b.min.x - m_border, b.max.y, 0 );
		rectangle->render( renderer );
		
		p->writable()[0] = V3f( b.max.x, b.min.y, 0 );
		p->writable()[1] = V3f( b.max.x + m_border, b.min.y, 0 );
		p->writable()[2] = V3f( b.max.x + m_border, b.max.y, 0 );
		p->writable()[3] = V3f( b.max.x, b.max.y, 0 );
		rectangle->render( renderer );
		
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
		renderer->transformEnd();*/
	
	renderer->attributeEnd();
	
	IndividualContainer::doRender( renderer );
}
