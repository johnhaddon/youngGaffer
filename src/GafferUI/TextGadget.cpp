#include "GafferUI/TextGadget.h"

using namespace GafferUI;

TextGadget::TextGadget( IECore::FontPtr font, const std::string &text )
	:	m_font( font ), m_text( text )
{
}

TextGadget::~TextGadget()
{
}

IECore::FontPtr TextGadget::getFont()
{
	return m_font;
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
	Imath::Box2f b = m_font->bound( m_text );
	return Imath::Box3f( Imath::V3f( b.min.x, b.min.y, 0 ), Imath::V3f( b.max.x, b.max.y, 0 ) );
}

void TextGadget::doRender( IECore::RendererPtr renderer ) const
{
	renderer->text( m_font->fileName(), m_text );
}
