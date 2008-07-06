#include "GafferUI/Nodule.h"
#include "GafferUI/Style.h"

#include "Gaffer/Plug.h"

using namespace GafferUI;
using namespace Imath;

Nodule::Nodule( Gaffer::PlugPtr plug )
	:	Gadget( staticTypeName() ), m_plug( plug )
{
}

Nodule::~Nodule()
{
}

Gaffer::PlugPtr Nodule::plug()
{
	return m_plug;
}

Gaffer::ConstPlugPtr Nodule::plug() const
{
	return m_plug;
}

Imath::Box3f Nodule::bound() const
{
	return Box3f( V3f( -0.2, -0.2, 0 ), V3f( 0.2, 0.2, 0 ) );
}

void Nodule::doRender( IECore::RendererPtr renderer ) const
{
	getStyle()->renderNodule( renderer, 0.2 );
}
