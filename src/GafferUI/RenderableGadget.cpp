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
	return m_renderable->bound();
}

void RenderableGadget::doRender( IECore::RendererPtr renderer ) const
{
	m_renderable->render( renderer );
}

void RenderableGadget::setRenderable( IECore::VisibleRenderablePtr renderable )
{
	m_renderable = renderable;
}

IECore::VisibleRenderablePtr RenderableGadget::getRenderable()
{
	return m_renderable;
}

IECore::ConstVisibleRenderablePtr RenderableGadget::getRenderable() const
{
	return m_renderable;
}
