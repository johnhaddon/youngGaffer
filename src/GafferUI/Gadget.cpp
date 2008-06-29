#include "GafferUI/Gadget.h"
#include "GafferUI/Style.h"

#include "IECore/SimpleTypedData.h"

#include <sstream>

using namespace GafferUI;
using namespace Imath;
using namespace std;

Gadget::Gadget( const std::string &name )
	:	GraphComponent( name ), m_style( Style::getDefaultStyle() )
{
}

Gadget::~Gadget()
{
}

bool Gadget::acceptsChild( Gaffer::ConstGraphComponentPtr potentialChild ) const
{
	return false;
}

bool Gadget::acceptsParent( const Gaffer::GraphComponent *potentialParent ) const
{
	return potentialParent->isInstanceOf( staticTypeId() );
}

ConstStylePtr Gadget::getStyle() const
{
	return m_style;
}

void Gadget::setStyle( ConstStylePtr style )
{
	if( style!=m_style )
	{
		m_style = style;
		renderRequestSignal()( this );
	}
}

void Gadget::render( IECore::RendererPtr renderer ) const
{
	renderer->setAttribute( "name", new IECore::StringData( fullName() ) );
	doRender( renderer );
}

Gadget::RenderRequestSignal &Gadget::renderRequestSignal()
{
	return m_renderRequestSignal;
}

Gadget::ButtonSignal &Gadget::buttonPressSignal()
{
	return m_buttonPressSignal;
}

Gadget::ButtonSignal &Gadget::buttonReleaseSignal()
{
	return m_buttonReleaseSignal;
}

Gadget::ButtonSignal &Gadget::dragBeginSignal()
{
	return m_dragBeginSignal;
}

Gadget::ButtonSignal &Gadget::dragUpdateSignal()
{
	return m_dragUpdateSignal;
}

Gadget::ButtonSignal &Gadget::dropSignal()
{
	return m_dropSignal;
}
		
Gadget::KeySignal &Gadget::keyPressSignal()
{
	return m_keyPressSignal;
}

Gadget::KeySignal &Gadget::keyReleaseSignal()
{
	return m_keyReleaseSignal;
}
