#ifndef GAFFERUI_TEXTGADGET_H
#define GAFFERUI_TEXTGADGET_H

#include "GafferUI/Gadget.h"

#include "IECore/Font.h"

namespace GafferUI
{

class TextGadget : public Gadget
{

	public :

		TextGadget( const std::string &text );
		virtual ~TextGadget();

		IE_CORE_DECLARERUNTIMETYPEDEXTENSION( TextGadget, TextGadgetTypeId, Gadget );

		/// Returns the font that will be used to display the text.
		/// If this has not been set it will default to getStyle()->labelFont().
		/// \todo This should return ConstFontPtr, but there are non-const methods
		/// which should be const in IECore::Font.
		IECore::FontPtr getFont() const;
		/// Sets the font used to display the text - this overrides the font
		/// set by the Style. Passing 0 reverts to the font from the Style.
		void setFont( IECore::FontPtr font );
		
		const std::string &getText() const;
		void setText( const std::string &text );

		virtual Imath::Box3f bound() const;

	protected :
	
		virtual void doRender( IECore::RendererPtr renderer ) const;

	private :
		
		IECore::FontPtr m_font;
		std::string m_text;
		
};

IE_CORE_DECLAREPTR( TextGadget );

} // namespace GafferUI

#endif // GAFFERUI_TEXTGADGET_H
