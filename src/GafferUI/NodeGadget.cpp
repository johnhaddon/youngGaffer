#include "GafferUI/NodeGadget.h"
#include "GafferUI/NameGadget.h"
#include "GafferUI/Style.h"

#include "IECore/SimpleTypedData.h"

#include "boost/bind.hpp"

using namespace GafferUI;
using namespace Imath;
using namespace IECore;
using namespace std;

NodeGadget::NodeGadget( Gaffer::NodePtr node )
	:	Frame( new NameGadget( node ) ), m_node( node.get() )
{
	
	Gaffer::ScriptNodePtr script = node->scriptNode();
	if( script )
	{
		script->selection()->memberAddedSignal().connect( boost::bind( &NodeGadget::selectionChanged, this, ::_1,  ::_2 ) );
		script->selection()->memberRemovedSignal().connect( boost::bind( &NodeGadget::selectionChanged, this, ::_1,  ::_2 ) );
	}
	
	buttonPressSignal().connect( boost::bind( &NodeGadget::buttonPressed, this, ::_1,  ::_2 ) );
}

NodeGadget::~NodeGadget()
{
}

Gaffer::NodePtr NodeGadget::node()
{
	return m_node;
}

Gaffer::ConstNodePtr NodeGadget::node() const
{
	return m_node;
}

bool NodeGadget::acceptsChild( Gaffer::ConstGraphComponentPtr potentialChild ) const
{
	return children().size()==0;
}

void NodeGadget::doRender( IECore::RendererPtr renderer ) const
{
	/// \todo Sort this mess out - where is the push/pop of attributes for a start?
	Gaffer::ScriptNodePtr script = m_node->scriptNode();
	if( script && script->selection()->contains( m_node ) )
	{
		/// \todo The Style should be doing this or the frame should have a colour member
		/// that can be set (and which falls through to style?)
		renderer->setAttribute( "color", new Color3fData( Color3f( 1 ) ) );
	}
	else
	{
		renderer->setAttribute( "color", new Color3fData( Color3f( 0.2 ) ) );
	}
	Frame::doRender( renderer );
}

bool NodeGadget::buttonPressed( GadgetPtr gadget, const ButtonEvent &event )
{
	if( event.buttons==ButtonEvent::Left )
	{
		// selection
		Gaffer::ScriptNodePtr script = m_node->scriptNode();
		if( !script )
		{
			return false;
		}
		bool shiftHeld = event.modifiers && ButtonEvent::Shift;
		if( !shiftHeld )
		{
			script->selection()->clear();
		}
		if( shiftHeld && script->selection()->contains( m_node ) )
		{
			script->selection()->remove( m_node );
		}
		else
		{
			script->selection()->add( m_node );
		}
		return true;
	}
	return false;
}

void NodeGadget::selectionChanged( Gaffer::NodeSetPtr selection, Gaffer::NodePtr node )
{
	if( node==m_node )
	{
		renderRequestSignal()( this );
	}
}
