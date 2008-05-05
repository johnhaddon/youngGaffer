#include "GafferUI/ContainerGadget.h"

#include "OpenEXR/ImathBoxAlgo.h"

using namespace GafferUI;

ContainerGadget::ContainerGadget( const std::string &name )
	:	Gadget( name )
{
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
		Imath::transform( b, m );
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
