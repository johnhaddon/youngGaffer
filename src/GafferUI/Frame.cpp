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
		
		getStyle()->renderFrame( renderer, Box2f( V2f( b.min.x, b.min.y ), V2f( b.max.x, b.max.y ) ), m_border );
	
	renderer->attributeEnd();
	
	IndividualContainer::doRender( renderer );
}
