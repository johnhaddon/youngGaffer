#include "GafferUI/NodeGadget.h"

#include "IECore/MeshPrimitive.h"

using namespace GafferUI;
using namespace Imath;
using namespace IECore;

NodeGadget::NodeGadget()
{
}

NodeGadget::~NodeGadget()
{
}

void NodeGadget::doRender( IECore::RendererPtr renderer ) const
{
	MeshPrimitivePtr plane = MeshPrimitive::createPlane( Box2f( V2f( -0.5, -0.5 ), V2f( 0.5, 0.5 ) ) );
	plane->render( renderer );
}

Imath::Box3f NodeGadget::bound( IECore::RendererPtr renderer ) const
{
	return Box3f( V3f( -1, -1, 0 ), V3f( 1, 1, 0 ) );
}
