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

IE_CORE_DEFINERUNTIMETYPED( NodeGadget );

static const float g_borderWidth = 0.25;
static const float g_minWidth = 10;

NodeGadget::NodeGadget( Gaffer::NodePtr node )
	:	m_node( node.get() )
{
	LinearContainerPtr column = new LinearContainer( "column", LinearContainer::Y, LinearContainer::Centre, 0.5f );
	
	LinearContainerPtr inputNoduleRow = new LinearContainer( "inputNoduleRow", LinearContainer::X, LinearContainer::Centre, 2.0f );
	LinearContainerPtr outputNoduleRow = new LinearContainer( "outputNoduleRow", LinearContainer::X, LinearContainer::Centre, 2.0f );
	
	for( Gaffer::PlugIterator it=node->plugsBegin(); it!=node->plugsEnd(); it++ )
	{
		Gaffer::ConstPlugPtr p = *it;
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

Imath::Box3f NodeGadget::bound() const
{
	Box3f b = IndividualContainer::bound();
	float width = std::max( b.size().x + 2 * g_borderWidth, g_minWidth );
	float c = b.center().x;
	b.min.x = c - width / 2.0f;
	b.max.x = c + width / 2.0f;
	return b;
}

void NodeGadget::doRender( IECore::RendererPtr renderer ) const
{
	
	renderer->attributeBegin();
	
		Gaffer::ScriptNodePtr script = m_node->scriptNode();
		if( script && script->selection()->contains( m_node ) )
		{
			renderer->setAttribute( Style::stateAttribute(), Style::stateValueSelected() );
		}
		else
		{
			renderer->setAttribute( Style::stateAttribute(), Style::stateValueNormal() );		
		}
		
		Box3f b = bound();
		Box3f inputRowBound = inputNoduleRow()->transformedBound( this );
		Box3f outputRowBound = outputNoduleRow()->transformedBound( this );
		
		float frameTop = inputRowBound.center().y;
		float frameBottom = outputRowBound.center().y;
		
		getStyle()->renderFrame( renderer, Box2f( V2f( b.min.x + g_borderWidth, frameBottom + g_borderWidth ), V2f( b.max.x - g_borderWidth, frameTop - g_borderWidth ) ), g_borderWidth );
		
		IndividualContainer::doRender( renderer );

	renderer->attributeEnd();

}

void NodeGadget::selectionChanged( Gaffer::NodeSetPtr selection, Gaffer::NodePtr node )
{
	if( node==m_node )
	{
		renderRequestSignal()( this );
	}
}

ConstLinearContainerPtr NodeGadget::inputNoduleRow() const
{
	return getChild<Gadget>()->getChild<LinearContainer>( "inputNoduleRow" );
}

ConstLinearContainerPtr NodeGadget::outputNoduleRow() const
{
	return getChild<Gadget>()->getChild<LinearContainer>( "outputNoduleRow" );
}
