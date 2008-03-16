#include "GafferUI/GraphGadget.h"
#include "GafferUI/NodeGadget.h"

#include "Gaffer/NumericPlug.h"

#include "IECore/MeshPrimitive.h"

#include "OpenEXR/ImathRandom.h"

#include "boost/bind.hpp"
#include "boost/bind/placeholders.hpp"

using namespace GafferUI;
using namespace Imath;
using namespace IECore;

GraphGadget::GraphGadget( Gaffer::NodePtr parent )
{
	
	parent->childAddedSignal().connect( boost::bind( &GraphGadget::childAdded, this, ::_1,  ::_2 ) );

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

static Imath::Rand32 r;

void GraphGadget::childAdded( GraphComponent *parent, GraphComponent *child )
{
	std::cerr << "CHILD ADDED !! " << parent->getName() << " " << child->getName() << std::endl;
	
	Gaffer::FloatPlugPtr x = new Gaffer::FloatPlug( "__uiX" );
	x->setValue( r.nextf( -1, 1 ) );
	
	Gaffer::FloatPlugPtr y = new Gaffer::FloatPlug( "__uiY" );
	y->setValue( r.nextf( -1, 1 ) );
	
	child->addChild( x );
	child->addChild( y );
	
	addChild( new NodeGadget( static_cast<Gaffer::Node *>( child ) ) );
	
}
