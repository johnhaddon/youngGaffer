#include "GafferUI/RenderableGadget.h"

#include "IECore/SimpleTypedData.h"

#include <sstream>

using namespace GafferUI;
using namespace Imath;
using namespace std;

RenderableGadget::RenderableGadget( IECore::VisibleRenderablePtr renderable )
	:	Gadget( staticTypeName() ), m_renderable( renderable )
{
}

RenderableGadget::~RenderableGadget()
{
}

Imath::Box3f RenderableGadget::bound() const
{
	if( m_renderable )
	{
		return m_renderable->bound();
	}
	else
	{
		return Imath::Box3f();
	}
}

void RenderableGadget::doRender( IECore::RendererPtr renderer ) const
{
	if( m_renderable )
	{
		m_renderable->render( renderer );
	}
}

void RenderableGadget::setRenderable( IECore::VisibleRenderablePtr renderable )
{
	if( renderable!=m_renderable )
	{
		m_renderable = renderable;
		renderRequestSignal()( this );
	}
}

IECore::VisibleRenderablePtr RenderableGadget::getRenderable()
{
	return m_renderable;
}

IECore::ConstVisibleRenderablePtr RenderableGadget::getRenderable() const
{
	return m_renderable;
}
