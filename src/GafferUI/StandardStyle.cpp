#include "GafferUI/StandardStyle.h"

#include "IECore/MeshPrimitive.h"
#include "IECore/CurvesPrimitive.h"
#include "IECore/SimpleTypedData.h"
#include "IECore/ObjectWriter.h"
#include "IECore/FileNameParameter.h"

using namespace GafferUI;
using namespace IECore;
using namespace Imath;
using namespace std;

IE_CORE_DEFINERUNTIMETYPED( StandardStyle );

StandardStyle::StandardStyle()
{
}

StandardStyle::~StandardStyle()
{
}


IECore::FontPtr StandardStyle::labelFont() const
{
	/// \todo Get font from somewhere within the $GAFFER_ROOT
	static FontPtr f = new Font( "/usr/X11R6/lib/X11/fonts/TTF/Vera.ttf" );
	return f;
}

void StandardStyle::renderFrame( IECore::RendererPtr renderer, const Imath::Box2f &frame, float borderWidth ) const
{
	static MeshPrimitivePtr quad = MeshPrimitive::createPlane( Box2f( V2f( 0 ), V2f( 1 ) ) );
	renderer->attributeBegin();
	
		renderer->setAttribute( "color", new Color3fData( backgroundColor( renderer ) ) );
	
		CompoundDataMap parameters;
		parameters["cornerSizes"] = new IECore::V2fData( Imath::V2f(
			borderWidth / ( frame.size().x + 2 * borderWidth ),
			borderWidth / ( frame.size().y + 2 * borderWidth )
		) );
		renderer->shader( "surface", "ui/frame", parameters );
	
		M44f m;
		m.setTranslation( V3f( frame.min.x - borderWidth, frame.min.y - borderWidth, 0 ) );
		m.scale( V3f( frame.size().x + 2 * borderWidth, frame.size().y + 2 * borderWidth, 1 ) );
		renderer->concatTransform( m );
		
		quad->render( renderer );
	
	renderer->attributeEnd();
}

void StandardStyle::renderNodule( IECore::RendererPtr renderer, float radius ) const
{
	static MeshPrimitivePtr quad = MeshPrimitive::createPlane( Box2f( V2f( -1 ), V2f( 1 ) ) );
	renderer->attributeBegin();
	
		renderer->shader( "surface", "ui/nodule", CompoundDataMap() );
	
		M44f m;
		m.scale( V3f( radius ) );
		renderer->concatTransform( m );
		
		quad->render( renderer );
	
	renderer->attributeEnd();
}

void StandardStyle::renderConnection( IECore::RendererPtr renderer, const Imath::V3f &src, const Imath::V3f &dst ) const
{
	static V3fVectorDataPtr p = 0;
	static CurvesPrimitivePtr c = 0;
	if( !c )
	{
		p = new V3fVectorData;
		p->writable().resize( 4 );
		IntVectorDataPtr v = new IntVectorData;
		v->writable().push_back( 4 );
		c = new CurvesPrimitive( v, CubicBasisf::bezier(), false );
		c->variables["P"] = PrimitiveVariable( PrimitiveVariable::Vertex, p );
		c->variables["width"] = PrimitiveVariable( PrimitiveVariable::Constant, new FloatData( 0.1 ) );
	}
	
	vector<V3f> &pp = p->writable();
	
	float d = (src.y - dst.y) / 2.0f;
	pp[0] = V3f( src.x, src.y, 0 );
	pp[1] = V3f( src.x, src.y - d, 0 );
	pp[2] = V3f( dst.x, dst.y + d, 0 );
	pp[3] = V3f( dst.x, dst.y, 0 );
		
	renderer->attributeBegin();
	
		renderer->shader( "surface", "ui/connection", CompoundDataMap() );
		c->render( renderer );
		
	renderer->attributeEnd();
	
}

Imath::Color3f StandardStyle::backgroundColor( IECore::RendererPtr renderer ) const
{
	ConstDataPtr state = renderer->getAttribute( stateAttribute() );
	if( state && state->isEqualTo( stateValueNormal() ) )
	{
		return Color3f( 0.4 );
	}
	else
	{
		return Color3f( 0.8 );
	}
}

Imath::Color3f StandardStyle::foregroundColor( IECore::RendererPtr renderer ) const
{
	return Color3f( 0 );
}
