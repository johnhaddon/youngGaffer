#include "GafferUI/NodeGadget.h"
#include "GafferUI/NameGadget.h"
#include "GafferUI/Style.h"
#include "GafferUI/LinearContainer.h"
#include "GafferUI/Nodule.h"

#include "Gaffer/ScriptNode.h"

#include "IECore/SimpleTypedData.h"

#include "boost/bind.hpp"

using namespace GafferUI;
using namespace Imath;
using namespace IECore;
using namespace std;
using namespace boost;

NodeGadget::NodeGadget( Gaffer::NodePtr node )
	:	m_node( node.get() )
{
	LinearContainerPtr column = new LinearContainer( "column", LinearContainer::Y, LinearContainer::Centre, 0.1f );
	
	LinearContainerPtr inputNoduleRow = new LinearContainer( "inputNoduleRow", LinearContainer::X, LinearContainer::Centre, 0.3f );
	LinearContainerPtr outputNoduleRow = new LinearContainer( "outputNoduleRow", LinearContainer::X, LinearContainer::Centre, 0.3f );
	
	for( Gaffer::PlugIterator it=node->plugsBegin(); it!=node->plugsEnd(); it++ )
	{
		Gaffer::ConstPlugPtr p = static_pointer_cast<const Gaffer::Plug>( *it );
		if( p->getName().compare( 0, 2, "__" ) )
		{
			/// \todo We need a totally customisable way of saying whether a plug is represented with a nodule or not
			NodulePtr nodule = new Nodule( const_pointer_cast<Gaffer::Plug>( p ) );
			if( p->direction()==Gaffer::Plug::In )
			{
				inputNoduleRow->addChild( nodule );
			}
			else
			{
				outputNoduleRow->addChild( nodule );
			}
			m_nodules[p.get()] = nodule.get();
		}
	}
	
	column->addChild( outputNoduleRow );
	column->addChild( new NameGadget( node ) );
	column->addChild( inputNoduleRow );
	
	setChild( column );
	
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

NodulePtr NodeGadget::nodule( Gaffer::ConstPlugPtr plug )
{
	NoduleMap::iterator it = m_nodules.find( plug.get() );
	if( it==m_nodules.end() )
	{
		return 0;
	}
	return it->second;
}

ConstNodulePtr NodeGadget::nodule( Gaffer::ConstPlugPtr plug ) const
{
	NoduleMap::const_iterator it = m_nodules.find( plug.get() );
	if( it==m_nodules.end() )
	{
		return 0;
	}
	return it->second;
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
	IndividualContainer::doRender( renderer );
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
