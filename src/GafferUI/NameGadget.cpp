#include "GafferUI/NameGadget.h"

#include "boost/bind.hpp"
#include "boost/bind/placeholders.hpp"

using namespace GafferUI;

IE_CORE_DEFINERUNTIMETYPED( NameGadget );

NameGadget::NameGadget( Gaffer::GraphComponentPtr object )
	:	TextGadget( object->getName() )
{
	object->nameChangedSignal().connect( boost::bind( &NameGadget::nameChanged, this, ::_1 ) );
}

NameGadget::~NameGadget()
{
}

void NameGadget::nameChanged( Gaffer::GraphComponentPtr object )
{
	setText( object->getName() );
}
