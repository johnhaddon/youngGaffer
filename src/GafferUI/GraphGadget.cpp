#include "GafferUI/GraphGadget.h"
#include "GafferUI/NodeGadget.h"
#include "GafferUI/ButtonEvent.h"
#include "GafferUI/Nodule.h"
#include "GafferUI/ConnectionGadget.h"

#include "Gaffer/ScriptNode.h"
#include "Gaffer/NumericPlug.h"
#include "Gaffer/Set.h"

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
	buttonPressSignal().connect( boost::bind( &GraphGadget::buttonPress, this, ::_1,  ::_2 ) );
	buttonReleaseSignal().connect( boost::bind( &GraphGadget::buttonRelease, this, ::_1,  ::_2 ) );
	dragBeginSignal().connect( boost::bind( &GraphGadget::dragBegin, this, ::_1, ::_2 ) );
	dragUpdateSignal().connect( boost::bind( &GraphGadget::dragUpdate, this, ::_1, ::_2 ) );
}

GraphGadget::~GraphGadget()
{
}

bool GraphGadget::keyPressed( GadgetPtr gadget, const KeyEvent &event )
{
	cerr << "KEYPRESS" << endl;
	return false;
}

static Imath::Rand32 r;
void GraphGadget::childAdded( GraphComponent *parent, GraphComponent *child )
{
	Gaffer::Node *node = static_cast<Gaffer::Node *>( child );

	/// \todo Use a V2f plug when we get one
	Gaffer::FloatPlugPtr xPlug = new Gaffer::FloatPlug( "__uiX" );
	float x = r.nextf( -10, 10 );
	xPlug->setValue( x );
	
	Gaffer::FloatPlugPtr yPlug = new Gaffer::FloatPlug( "__uiY" );
	float y = r.nextf( -10, 10 );
	yPlug->setValue( y );
	
	node->addChild( xPlug );
	node->addChild( yPlug );
	
	node->plugInputChangedSignal().connect( boost::bind( &GraphGadget::inputChanged, this, ::_1 ) );
	
	NodeGadgetPtr nodeGadget = new NodeGadget( node );
	M44f m; m.translate( V3f( x, y, 0 ) );
	nodeGadget->setTransform( m );
	
	addChild( nodeGadget );
	m_nodeGadgets[node] = nodeGadget.get();
}

void GraphGadget::childRemoved( GraphComponent *parent, GraphComponent *child )
{
	
}

void GraphGadget::inputChanged( Gaffer::PlugPtr dstPlug )
{
	ConnectionGadget *oldConnection = connectionGadget( dstPlug.get() );
	if( oldConnection )
	{
		m_connectionGadgets.erase( dstPlug.get() );
		removeChild( oldConnection );
	}

	Gaffer::PlugPtr srcPlug = dstPlug->getInput<Gaffer::Plug>();
	if( !srcPlug )
	{
		// it's a disconnection, no need to make a new gadget.
		return;
	}
	
	Gaffer::NodePtr dstNode = dstPlug->node();
	Gaffer::NodePtr srcNode = srcPlug->node();
	
	NodulePtr srcNodule = nodeGadget( srcNode.get() )->nodule( srcPlug );
	NodulePtr dstNodule = nodeGadget( dstNode.get() )->nodule( dstPlug );
	
	if( !(srcNodule && dstNodule ) )
	{
		return;
	}
	
	ConnectionGadgetPtr connection = new ConnectionGadget( srcNodule, dstNodule );
	addChild( connection );

	m_connectionGadgets[dstPlug.get()] = connection.get();
}

bool GraphGadget::buttonRelease( GadgetPtr gadget, const ButtonEvent &event )
{
	std::cerr << "BUTTON RELEASE" << std::endl;
	return true;
}

bool GraphGadget::buttonPress( GadgetPtr gadget, const ButtonEvent &event )
{
	if( gadget->isInstanceOf( Nodule::staticTypeId() ) || gadget->isInstanceOf( ConnectionGadget::staticTypeId() ) )
	{
		// they can handle their own stuff.
		return false;
	}

	if( event.buttons==ButtonEvent::Left )
	{
		NodeGadgetPtr nodeGadget = runTimeCast<NodeGadget>( gadget );
		if( !nodeGadget )
		{
			nodeGadget = gadget->ancestor<NodeGadget>();
		}
		if( nodeGadget )
		{
			// selection
			Gaffer::ScriptNodePtr scriptNode = script();
			if( !scriptNode )
			{
				return false;
			}
			
			Gaffer::NodePtr node = nodeGadget->node();
			
			bool shiftHeld = event.modifiers && ButtonEvent::Shift;
			if( !shiftHeld )
			{
				scriptNode->selection()->clear();
			}
			if( shiftHeld && scriptNode->selection()->contains( node ) )
			{
				scriptNode->selection()->remove( node );
			}
			else
			{
				scriptNode->selection()->add( node );
			}
			return true;
		}
	}
	return false;
}

IECore::RunTimeTypedPtr GraphGadget::dragBegin( GadgetPtr gadget, const ButtonEvent &event )
{
	if( gadget->isInstanceOf( Nodule::staticTypeId() ) )
	{
		// nodules can handle their own drag and drop so we don't handle the event
		return 0;
	}
	
	Gaffer::ScriptNodePtr scriptNode = script();
	if( !scriptNode )
	{
		return 0;
	}
	
	V3f i;
	if( event.line.intersect( Plane3f( V3f( 0, 0, 1 ), 0 ), i ) )
	{
		m_lastDragPosition = V2f( i.x, i.y );
		return scriptNode->selection();
	}
	return 0;
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
						V3f p( xp->getValue() + delta.x, yp->getValue() + delta.y, 0.0f );
						xp->setValue( p.x );
						yp->setValue( p.y );
						M44f m; m.translate( p );
						nodeGadget->setTransform( m );
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

Gaffer::ScriptNodePtr GraphGadget::script()
{
	Gaffer::ScriptNodePtr script = runTimeCast<Gaffer::ScriptNode>( m_graphRoot );
	if( !script )
	{
		script = m_graphRoot->scriptNode();
	}
	return script;
}

NodeGadget *GraphGadget::nodeGadget( Gaffer::Node *node )
{
	NodeGadgetMap::iterator it = m_nodeGadgets.find( node );
	if( it==m_nodeGadgets.end() )
	{
		return 0;
	}
	return it->second;
}

ConnectionGadget *GraphGadget::connectionGadget( Gaffer::Plug *plug )
{
	ConnectionGadgetMap::iterator it = m_connectionGadgets.find( plug );
	if( it==m_connectionGadgets.end() )
	{
		return 0;
	}
	return it->second;
}
