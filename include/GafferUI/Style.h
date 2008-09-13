#ifndef GAFFERUI_STYLE_H
#define GAFFERUI_STYLE_H

#include "GafferUI/TypeIds.h"

#include "IECore/Renderer.h"
#include "IECore/Font.h"

namespace GafferUI
{

IE_CORE_FORWARDDECLARE( Style );

class Style : public IECore::RunTimeTyped
{

	public :

		Style();
		virtual ~Style();

		IE_CORE_DECLARERUNTIMETYPEDEXTENSION( Style, StyleTypeId, IECore::RunTimeTyped );

		virtual IECore::FontPtr labelFont() const = 0;	

		virtual void renderFrame( IECore::RendererPtr renderer, const Imath::Box2f &frame, float borderWidth ) const = 0;
		virtual void renderNodule( IECore::RendererPtr renderer, float radius ) const = 0;
		virtual void renderConnection( IECore::RendererPtr renderer, const Imath::V2f &src, const Imath::V2f &dst ) const = 0;
	
		//! @name Default style
		/// There always exists a default style which is
		/// applied to all newly created Gadgets. Typically
		/// you would set this once when an application
		/// starts and then leave it alone - if not set it
		/// defaults to an instance of StandardStyle.
		////////////////////////////////////////////////
		//@{
		static StylePtr getDefaultStyle();
		static void setDefaultStyle( StylePtr style );
		//@}
		
	private :
	
		static StylePtr g_defaultStyle;
		
};

} // namespace GafferUI

#endif // GAFFERUI_STYLE_H
