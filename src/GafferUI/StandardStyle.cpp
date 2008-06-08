#include "GafferUI/StandardStyle.h"

#include "IECore/MeshPrimitive.h"

using namespace GafferUI;
using namespace IECore;
using namespace Imath;

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
	
		renderer->shader( "surface", "ui/frame", CompoundDataMap() );
	
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
