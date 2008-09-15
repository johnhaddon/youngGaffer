#include "GafferUI/ConnectionGadget.h"
#include "GafferUI/GraphGadget.h"
#include "GafferUI/Style.h"
#include "GafferUI/Nodule.h"

using namespace GafferUI;
using namespace Imath;
using namespace std;

ConnectionGadget::ConnectionGadget( GafferUI::NodulePtr srcNodule, GafferUI::NodulePtr dstNodule )
	:	Gadget( staticTypeName() ), m_srcNodule( srcNodule ), m_dstNodule( dstNodule )
{
	setPositionsFromNodules();
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
	ConstGadgetPtr p = parent<Gadget>();
	if( m_srcNodule )
	{
		M44f m = m_srcNodule->fullTransform( p );
		m_srcPos = V3f( 0 ) * m;
	}
	if( m_dstNodule )
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
