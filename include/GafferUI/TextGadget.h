#ifndef GAFFERUI_TEXTGADGET_H
#define GAFFERUI_TEXTGADGET_H

#include "GafferUI/Gadget.h"

#include "IECore/Font.h"

namespace GafferUI
{

class TextGadget : public Gadget
{

	public :

		TextGadget( IECore::FontPtr font, const std::string &text );
		virtual ~TextGadget();

		IE_CORE_DECLARERUNTIMETYPEDEXTENSION( TextGadget, TextGadgetTypeId, Gadget );

		IECore::FontPtr getFont();
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
