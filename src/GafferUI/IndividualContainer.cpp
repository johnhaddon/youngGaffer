#include "GafferUI/IndividualContainer.h"

using namespace GafferUI;

IndividualContainer::IndividualContainer( GadgetPtr child )
	:	ContainerGadget( staticTypeName() )
{
	setChild( child );
}

IndividualContainer::~IndividualContainer()
{
}

bool IndividualContainer::acceptsChild( Gaffer::ConstGraphComponentPtr potentialChild ) const
{
	if( !ContainerGadget::acceptsChild( potentialChild ) )
	{
		return false;
	}
	return children().size()==0;
}

Imath::M44f IndividualContainer::childTransform( ConstGadgetPtr child ) const
{
	return Imath::M44f();
}

void IndividualContainer::setChild( GadgetPtr child )
{
	GadgetPtr c = getChild<Gadget>();
	if( c )
	{
		removeChild( c );
	}
	addChild( child );
}
