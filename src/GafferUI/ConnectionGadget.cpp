#include "Gaffer/UndoContext.h"
#include "Gaffer/ScriptNode.h"

#include "GafferUI/ConnectionGadget.h"
#include "GafferUI/GraphGadget.h"
#include "GafferUI/Style.h"
#include "GafferUI/Nodule.h"

#include "boost/bind.hpp"
#include "boost/bind/placeholders.hpp"

using namespace GafferUI;
using namespace Imath;
using namespace std;

IE_CORE_DEFINERUNTIMETYPED( ConnectionGadget );

ConnectionGadget::ConnectionGadget( GafferUI::NodulePtr srcNodule, GafferUI::NodulePtr dstNodule )
	:	Gadget( staticTypeName() ), m_srcNodule( srcNodule ), m_dstNodule( dstNodule ),
		m_dragEnd( Gaffer::Plug::Invalid )
{
	setPositionsFromNodules();
	
	buttonPressSignal().connect( boost::bind( &ConnectionGadget::buttonPress, this, ::_1,  ::_2 ) );
	dragBeginSignal().connect( boost::bind( &ConnectionGadget::dragBegin, this, ::_1, ::_2 ) );
	dragUpdateSignal().connect( boost::bind( &ConnectionGadget::dragUpdate, this, ::_1, ::_2 ) );
	dragEndSignal().connect( boost::bind( &ConnectionGadget::dragEnd, this, ::_1, ::_2 ) );
}

ConnectionGadget::~ConnectionGadget()
{
}

bool ConnectionGadget::acceptsParent( const Gaffer::GraphComponent *potentialParent ) const
{
	if( !Gadget::acceptsParent( potentialParent ) )
	{
		return false;
	}
	return IECore::runTimeCast<const GraphGadget>( potentialParent );
}	

void ConnectionGadget::setSrcPos( const Imath::V3f &p )
{
	if( m_srcPos!=p )
	{
		m_srcPos = p;
		renderRequestSignal()( this );
	}
}
		
void ConnectionGadget::setDstPos( const Imath::V3f &p )
{
	if( m_dstPos!=p )
	{
		m_dstPos = p;
		renderRequestSignal()( this );
	}
}

void ConnectionGadget::setPositionsFromNodules()
{
	const Gadget *p = parent<Gadget>();
	if( m_srcNodule && m_dragEnd!=Gaffer::Plug::Out )
	{
		M44f m = m_srcNodule->fullTransform( p );
		m_srcPos = V3f( 0 ) * m;
	}
	if( m_dstNodule && m_dragEnd!=Gaffer::Plug::In )
	{
		M44f m = m_dstNodule->fullTransform( p );
		m_dstPos = V3f( 0 ) * m;
	}
}
		
Imath::Box3f ConnectionGadget::bound() const
{
	Box3f r;
	r.extendBy( m_srcPos );
	r.extendBy( m_dstPos );
	return r;
}

void ConnectionGadget::doRender( IECore::RendererPtr renderer ) const
{
	const_cast<ConnectionGadget *>( this )->setPositionsFromNodules();
	getStyle()->renderConnection( renderer, m_srcPos, m_dstPos );
}

bool ConnectionGadget::buttonPress( GadgetPtr gadget, const ButtonEvent &event )
{
	if( event.buttons==ButtonEvent::Left )
	{
		// we have to accept button presses so we can initiate dragging
		return true;
	}
	return false;
}

IECore::RunTimeTypedPtr ConnectionGadget::dragBegin( GadgetPtr gadget, const DragDropEvent &event )
{
	setPositionsFromNodules();
	float length = ( m_srcPos - m_dstPos ).length();
	
	float dSrc = event.line.distanceTo( m_srcPos );
	float dDst = event.line.distanceTo( m_dstPos );
	
	float dMin = min( dSrc, dDst );
	if( dMin < length / 3.0f )
	{
		// close enough to the ends to consider
		if( dSrc < dDst )
		{
			m_dragEnd = Gaffer::Plug::Out;
			return m_dstNodule->plug();
		}
		else
		{
			m_dragEnd = Gaffer::Plug::In;
			return m_srcNodule->plug();
		}
	}
	
	return 0;
}

bool ConnectionGadget::dragUpdate( GadgetPtr gadget, const DragDropEvent &event )
{
	if( m_dragEnd==Gaffer::Plug::Out )
	{
		m_srcPos = event.line.p0;
	}
	else
	{
		m_dstPos = event.line.p0;
	}
	renderRequestSignal()( this );
	return 0;
}

bool ConnectionGadget::dragEnd( GadgetPtr gadget, const DragDropEvent &event )
{
	bool shouldDisconnect = false;
	if( !event.destination )
	{
		// noone wanted the drop so we'll disconnect
		shouldDisconnect = true;
	}
	else
	{
		NodulePtr n = IECore::runTimeCast<Nodule>( event.destination );
		if( n && event.dropResult && n->plug()->direction()==Gaffer::Plug::In )
		{
			// it was dropped on a nodule and the nodule changed the connection
			// to point somewhere else.
			shouldDisconnect = true;
		}
	}
	if( shouldDisconnect )
	{
		Gaffer::UndoContext undoEnabler( m_dstNodule->plug()->ancestor<Gaffer::ScriptNode>() );
		m_dstNodule->plug()->setInput( 0 );
	}
	m_dragEnd = Gaffer::Plug::Invalid;
	renderRequestSignal()( this );
	return true;
}

