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
		Imath::M44f m = childTransform( c );
		b = Imath::transform( b, m );
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
		renderer->attributeBegin();
			renderer->concatTransform( childTransform( c ) );
			c->render( renderer );
		renderer->attributeEnd();
	}
}

void ContainerGadget::childAdded( GraphComponent *us, GraphComponent *child )
{
	static_cast<Gadget *>( child )->renderRequestSignal().connect( &childRenderRequest );
	ContainerGadget *p = static_cast<ContainerGadget *>( us );
	if( !p->renderRequestSignal().empty() )
	{
		// we only call if slots are attached - otherwise we can make this call during construction
		// if derived classes add children. what would then happen is we'd make a smart pointer to
		// p (this) during the signal call, and then the refcount would drop to zero after the call,
		// and we'd destruct before we even constructed - with very bad results.
		// the alternative to this is that we don't pass a smart pointer in the signal - this might
		// turn out to be a better option in the long run.
		p->renderRequestSignal()( p );
	}
}

void ContainerGadget::childRemoved( GraphComponent *us, GraphComponent *child )
{
	static_cast<Gadget *>( child )->renderRequestSignal().disconnect( &childRenderRequest );
}

void ContainerGadget::childRenderRequest( GadgetPtr child )
{
	ContainerGadgetPtr p = child->parent<ContainerGadget>();
	assert( p );
	p->renderRequestSignal()( p );
}
