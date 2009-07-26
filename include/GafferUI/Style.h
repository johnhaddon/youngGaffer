#ifndef GAFFERUI_STYLE_H
#define GAFFERUI_STYLE_H

#include "GafferUI/TypeIds.h"

#include "IECore/Renderer.h"
#include "IECore/Font.h"

#include <stack>

namespace GafferUI
{

IE_CORE_FORWARDDECLARE( Style );

class Style : public IECore::RunTimeTyped
{

	public :

		Style();
		virtual ~Style();

		IE_CORE_DECLARERUNTIMETYPEDEXTENSION( Style, StyleTypeId, IECore::RunTimeTyped );
		
		/// \todo Not just renderLabel?
		virtual IECore::FontPtr labelFont() const = 0;

		/// \todo Should all these be taking 3d arguments - no but 3d counterparts might be good.
		virtual void renderFrame( IECore::RendererPtr renderer, const Imath::Box2f &frame, float borderWidth ) const = 0;
		virtual void renderNodule( IECore::RendererPtr renderer, float radius ) const = 0;
		virtual void renderConnection( IECore::RendererPtr renderer, const Imath::V3f &src, const Imath::V3f &dst ) const = 0;
		virtual void renderHandle( IECore::RendererPtr renderer, const Imath::V3f &p ) const = 0;
	
		//! @name User attributes
		/// Additional control is provided over the rendering
		/// of ui elements by specifying user attribute values
		/// to the Renderer before calling the render methods
		/// above. These methods provide the names and data
		/// values to be used in such calls.
		////////////////////////////////////////////////
		//@{
		/// Returns the name of an attribute which can
		/// be used to specify the state of the ui being
		/// rendered. This allows Gadgets to be rendered
		/// as inactive, selected etc.
		static const std::string &stateAttribute();
		/// The attribute value to specify a normal render
		/// state.
		static IECore::ConstDataPtr stateValueNormal(); 
		/// The attribute value to specify an inactive render
		/// state.
		static IECore::ConstDataPtr stateValueInactive();
		/// The attribute value to specify an inactive render
		/// state.
		static IECore::ConstDataPtr stateValueSelected(); 
		//@}
	
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
