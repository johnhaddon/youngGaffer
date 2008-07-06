#include "GafferUI/GraphGadget.h"
#include "GafferUI/NodeGadget.h"
#include "GafferUI/ButtonEvent.h"

#include "Gaffer/NumericPlug.h"

#include "IECore/MeshPrimitive.h"

#include "OpenEXR/ImathRandom.h"
#include "OpenEXR/ImathPlane.h"

#include "boost/bind.hpp"
#include "boost/bind/placeholders.hpp"

using namespace GafferUI;
using namespace Imath;
using namespace IECore;
using namespace std;

GraphGadget::GraphGadget( Gaffer::NodePtr graphRoot )
	:	m_graphRoot( graphRoot.get() )
{
	
	graphRoot->childAddedSignal().connect( boost::bind( &GraphGadget::childAdded, this, ::_1,  ::_2 ) );
	graphRoot->childRemovedSignal().connect( boost::bind( &GraphGadget::childRemoved, this, ::_1,  ::_2 ) );

	keyPressSignal().connect( boost::bind( &GraphGadget::keyPressed, this, ::_1,  ::_2 ) );
	dragBeginSignal().connect( boost::bind( &GraphGadget::dragBegin, this, ::_1, ::_2 ) );
	dragUpdateSignal().connect( boost::bind( &GraphGadget::dragUpdate, this, ::_1, ::_2 ) );
}

GraphGadget::~GraphGadget()
{
}

Imath::M44f GraphGadget::childTransform( ConstGadgetPtr child ) const
{
	M44f result;
	ConstNodeGadgetPtr nodeGadget = runTimeCast<const NodeGadget>( child );
	if( nodeGadget )
	{
		result.translate(
			V3f( 
				nodeGadget->node()->getChild<const Gaffer::FloatPlug>( "__uiX" )->getValue(),
				nodeGadget->node()->getChild<const Gaffer::FloatPlug>( "__uiY" )->getValue(),
				0
			)
		);
	}
	return result;
}

bool GraphGadget::keyPressed( GadgetPtr gadget, const KeyEvent &event )
{
	cerr << "KEYPRESS" << endl;
	return false;
}

static Imath::Rand32 r;
void GraphGadget::childAdded( GraphComponent *parent, GraphComponent *child )
{
	/// \todo Use a V2f plug when we get one
	Gaffer::FloatPlugPtr x = new Gaffer::FloatPlug( "__uiX" );
	x->setValue( r.nextf( -10, 10 ) );
	
	Gaffer::FloatPlugPtr y = new Gaffer::FloatPlug( "__uiY" );
	y->setValue( r.nextf( -10, 10 ) );
	
	child->addChild( x );
	child->addChild( y );
	
	addChild( new NodeGadget( static_cast<Gaffer::Node *>( child ) ) );
}

void GraphGadget::childRemoved( GraphComponent *parent, GraphComponent *child )
{
	
}

bool GraphGadget::dragBegin( GadgetPtr gadget, const ButtonEvent &event )
{
	V3f i;
	if( event.line.intersect( Plane3f( V3f( 0, 0, 1 ), 0 ), i ) )
	{
		m_lastDragPosition = V2f( i.x, i.y );
		return true;
	}
	return false;
}

bool GraphGadget::dragUpdate( GadgetPtr gadget, const ButtonEvent &event )
{
	Gaffer::ScriptNodePtr script = runTimeCast<Gaffer::ScriptNode>( m_graphRoot );
	if( !script )
	{
		script = m_graphRoot->scriptNode();
	}
	if( script )
	{
		V3f i;
		if( event.line.intersect( Plane3f( V3f( 0, 0, 1 ), 0 ), i ) )
		{
			V2f pos = V2f( i.x, i.y );
			V2f delta = pos - m_lastDragPosition;
			for( ChildContainer::const_iterator it=children().begin(); it!=children().end(); it++ )
			{
				NodeGadgetPtr nodeGadget = runTimeCast<NodeGadget>( *it );
				if( nodeGadget )
				{
					Gaffer::NodePtr node = nodeGadget->node();
					if( script->selection()->contains( node ) )
					{
						Gaffer::FloatPlugPtr xp = node->getChild<Gaffer::FloatPlug>( "__uiX" );
						Gaffer::FloatPlugPtr yp = node->getChild<Gaffer::FloatPlug>( "__uiY" );
						xp->setValue( xp->getValue() + delta.x );
						yp->setValue( yp->getValue() + delta.y );
					}
				}
			}
			renderRequestSignal()( this ); /// \todo This should be coming from a plug changed callback on __uiX for all children
			m_lastDragPosition = pos;
			return true;
		}
	}
	return false;
}
