#ifndef GAFFERUI_STANDARDSTYLE_H
#define GAFFERUI_STANDARDSTYLE_H

#include "GafferUI/Style.h"

#include "IECore/Renderer.h"

namespace GafferUI
{

class StandardStyle : public Style
{

	public :

		StandardStyle();
		virtual ~StandardStyle();

		IE_CORE_DECLARERUNTIMETYPEDEXTENSION( StandardStyle, StandardStyleTypeId, Style );

		virtual IECore::FontPtr labelFont() const;

		virtual void renderFrame( IECore::RendererPtr renderer, const Imath::Box2f &frame, float borderWidth ) const;
		virtual void renderNodule( IECore::RendererPtr renderer, float radius ) const;
		virtual void renderConnection( IECore::RendererPtr renderer, const Imath::V3f &src, const Imath::V3f &dst ) const;
		virtual void renderHandle( IECore::RendererPtr renderer, const Imath::V3f &p ) const;

	private :
	
		/// \todo These should be hooked up to a preferences node somehow
		Imath::Color3f backgroundColor( IECore::RendererPtr renderer ) const;
		Imath::Color3f foregroundColor( IECore::RendererPtr renderer ) const;
		
};

IE_CORE_DECLAREPTR( Style );

} // namespace GafferUI

#endif // GAFFERUI_STANDARDSTYLE_H
