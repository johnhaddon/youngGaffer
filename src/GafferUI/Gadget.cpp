#include "GafferUI/Gadget.h"

#include "IECore/SimpleTypedData.h"

#include <sstream>

using namespace GafferUI;
using namespace Imath;
using namespace std;

Gadget::Gadget( const std::string &name )
	:	GraphComponent( name )
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

void Gadget::render( IECore::RendererPtr renderer ) const
{
	renderer->setAttribute( "name", new IECore::StringData( fullName() ) );
	doRender( renderer );
}

Gadget::ButtonSignal &Gadget::buttonPressSignal()
{
	return m_buttonPressSignal;
}

Gadget::ButtonSignal &Gadget::buttonReleaseSignal()
{
	return m_buttonReleaseSignal;
}

Gadget::KeySignal &Gadget::keyPressSignal()
{
	return m_keyPressSignal;
}

Gadget::KeySignal &Gadget::keyReleaseSignal()
{
	return m_keyReleaseSignal;
}
