#include "GafferUI/NodeGadget.h"
#include "GafferUI/NameGadget.h"

#include "IECore/MeshPrimitive.h"

using namespace GafferUI;
using namespace Imath;
using namespace IECore;

/// \todo I suspect there should be a central resource class providing the font to all gadgets.
NodeGadget::NodeGadget( Gaffer::NodePtr node )
	:	Frame( new NameGadget( new Font( "/usr/X11R6/lib/X11/fonts/TTF/Vera.ttf" ), node ) ), m_node( node.get() )
{
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
