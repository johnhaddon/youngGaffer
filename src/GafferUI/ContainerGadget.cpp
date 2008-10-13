#include "GafferUI/ContainerGadget.h"

#include "OpenEXR/ImathBoxAlgo.h"

#include "boost/bind.hpp"
#include "boost/bind/placeholders.hpp"

using namespace GafferUI;

ContainerGadget::ContainerGadget( const std::string &name )
	:	Gadget( name )
{
	childAddedSignal().connect( boost::bind( &ContainerGadget::childAdded, this, ::_1, ::_2 ) );
	childRemovedSignal().connect( boost::bind( &ContainerGadget::childRemoved, this, ::_1, ::_2 )  );
}

ContainerGadget::~ContainerGadget()
{
}

bool ContainerGadget::acceptsChild( Gaffer::ConstGraphComponentPtr potentialChild ) const
{
	return potentialChild->isInstanceOf( Gadget::staticTypeId() );
}

Imath::Box3f ContainerGadget::bound() const
{
	Imath::Box3f result;
	for( ChildContainer::const_iterator it=children().begin(); it!=children().end(); it++ )
	{
		// cast is safe because of the guarantees acceptsChild() gives us
		ConstGadgetPtr c = boost::static_pointer_cast<const Gadget>( *it );
		Imath::Box3f b = c->bound();
		b = Imath::transform( b, c->getTransform() );
		result.extendBy( b );
	}
	return result;
}

void ContainerGadget::doRender( IECore::RendererPtr renderer ) const
{
	for( ChildContainer::const_iterator it=children().begin(); it!=children().end(); it++ )
	{
		// cast is safe because of the guarantees acceptsChild() gives us
		ConstGadgetPtr c = boost::static_pointer_cast<const Gadget>( *it );
		c->render( renderer );
	}
}

void ContainerGadget::childAdded( GraphComponent *parent, GraphComponent *child )
{
	assert( parent==this );
	static_cast<Gadget *>( child )->renderRequestSignal().connect( boost::bind( &ContainerGadget::childRenderRequest, this, ::_1 ) );
	renderRequestSignal()( this );
}

void ContainerGadget::childRemoved( GraphComponent *parent, GraphComponent *child )
{
	assert( parent==this );
	static_cast<Gadget *>( child )->renderRequestSignal().disconnect( &ContainerGadget::childRenderRequest );
	renderRequestSignal()( this );
}

void ContainerGadget::childRenderRequest( Gadget *child )
{
	renderRequestSignal()( this );
}
