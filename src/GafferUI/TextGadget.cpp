#include "GafferUI/TextGadget.h"
#include "GafferUI/Style.h"

using namespace GafferUI;
using namespace IECore;
using namespace boost;

TextGadget::TextGadget( const std::string &text )
	:	Gadget( staticTypeName() ), m_font( 0 ), m_text( text )
{
}

TextGadget::~TextGadget()
{
}

IECore::FontPtr TextGadget::getFont() const
{
	return const_pointer_cast<Font>( m_font ? m_font : getStyle()->labelFont() );
}

void TextGadget::setFont( IECore::FontPtr font )
{
	m_font = font;
}

const std::string &TextGadget::getText() const
{
	return m_text;
}

void TextGadget::setText( const std::string &text )
{
	if( text!=m_text )
	{
		m_text = text;
		renderRequestSignal();
	}
}

Imath::Box3f TextGadget::bound() const
{
	Imath::Box2f b = getFont()->bound( m_text );
	return Imath::Box3f( Imath::V3f( b.min.x, b.min.y, 0 ), Imath::V3f( b.max.x, b.max.y, 0 ) );
}

void TextGadget::doRender( IECore::RendererPtr renderer ) const
{
	renderer->text( getFont()->fileName(), m_text );
}