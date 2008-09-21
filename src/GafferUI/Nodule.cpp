#include "GafferUI/Nodule.h"
#include "GafferUI/Style.h"

#include "Gaffer/Plug.h"

#include "boost/bind.hpp"
#include "boost/bind/placeholders.hpp"

using namespace GafferUI;
using namespace Imath;
using namespace std;

Nodule::Nodule( Gaffer::PlugPtr plug )
	:	Gadget( staticTypeName() ), m_plug( plug )
{
	buttonPressSignal().connect( boost::bind( &Nodule::buttonPress, this, ::_1,  ::_2 ) );
	dragBeginSignal().connect( boost::bind( &Nodule::dragBegin, this, ::_1, ::_2 ) );
	dropSignal().connect( boost::bind( &Nodule::drop, this, ::_1, ::_2 ) );
}

Nodule::~Nodule()
{
}

Gaffer::PlugPtr Nodule::plug()
{
	return m_plug;
}

Gaffer::ConstPlugPtr Nodule::plug() const
{
	return m_plug;
}

Imath::Box3f Nodule::bound() const
{
	return Box3f( V3f( -0.2, -0.2, 0 ), V3f( 0.2, 0.2, 0 ) );
}

void Nodule::doRender( IECore::RendererPtr renderer ) const
{
	getStyle()->renderNodule( renderer, 0.2 );
}

bool Nodule::buttonPress( GadgetPtr gadget, const ButtonEvent &event )
{
	// we handle the button press so we can get the dragBegin event.
	return true;
}

IECore::RunTimeTypedPtr Nodule::dragBegin( GadgetPtr gadget, const ButtonEvent &event )
{
	return m_plug;
}

bool Nodule::drop( GadgetPtr gadget, const DragDropEvent &event )
{
	Gaffer::PlugPtr plug = IECore::runTimeCast<Gaffer::Plug>( event.data );
	if( plug )
	{
		if( m_plug->direction()!=plug->direction() )
		{
			Gaffer::PlugPtr input = 0;
			Gaffer::PlugPtr output = 0;
			if( m_plug->direction()==Gaffer::Plug::In )
			{
				input = m_plug;
				output = plug;
			}
			else
			{
				input = plug;
				output = m_plug;
			}
			input->setInput( output );
			cerr << "CONNECTING " << output->fullName() << " TO " << input->fullName() << endl;
			return true;
		}
	}
	cerr << "RETURNING FALSE" << endl;
	return false;
}
