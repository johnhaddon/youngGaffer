#include "GafferUI/NodeGadget.h"
#include "GafferUI/NameGadget.h"
#include "GafferUI/Style.h"

#include "IECore/MeshPrimitive.h"

using namespace GafferUI;
using namespace Imath;
using namespace IECore;

NodeGadget::NodeGadget( Gaffer::NodePtr node )
	:	Frame( new NameGadget( node ) ), m_node( node.get() )
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
