#include "GafferUI/ContainerGadget.h"

#include "OpenEXR/ImathBoxAlgo.h"

using namespace GafferUI;

ContainerGadget::ContainerGadget( const std::string &name )
	:	Gadget( name )
{
	childAddedSignal().connect( &childAdded );
	childRemovedSignal().connect( &childRemoved );
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

void ContainerGadget::childAdded( GraphComponent *us, GraphComponent *child )
{
	static_cast<Gadget *>( child )->renderRequestSignal().connect( &childRenderRequest );
	ContainerGadget *p = static_cast<ContainerGadget *>( us );
	p->renderRequestSignal()( p );
}

void ContainerGadget::childRemoved( GraphComponent *us, GraphComponent *child )
{
	static_cast<Gadget *>( child )->renderRequestSignal().disconnect( &childRenderRequest );
	ContainerGadget *p = static_cast<ContainerGadget *>( us );
	p->renderRequestSignal()( p );
}

void ContainerGadget::childRenderRequest( GadgetPtr child )
{
	ContainerGadgetPtr p = child->parent<ContainerGadget>();
	assert( p );
	p->renderRequestSignal()( p.get() );
}
