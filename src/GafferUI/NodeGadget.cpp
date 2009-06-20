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

NodeGadget::CreatorMap NodeGadget::g_creators;

NodeGadget::NodeGadget( Gaffer::NodePtr node )
	:	m_node( node.get() )
{
}

NodeGadget::~NodeGadget()
{
}

NodeGadgetPtr NodeGadget::create( Gaffer::NodePtr node )
{
	CreatorMap::const_iterator it = g_creators.find( node->typeId() );
	if( it==g_creators.end() )
	{
		const std::vector<IECore::TypeId> &baseTypes = IECore::RunTimeTyped::baseTypeIds( node->typeId() );
		for( std::vector<IECore::TypeId>::const_iterator tIt=baseTypes.begin(); tIt!=baseTypes.end(); tIt++ )
		{
			if( ( it = g_creators.find( *tIt ) )!=g_creators.end() )
			{
				break;
			}
		}
	}
	
	return it->second( node );
}

void NodeGadget::registerNodeGadget( IECore::TypeId nodeType, NodeGadgetCreator creator )
{
	g_creators[nodeType] = creator;
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
	return 0;
}

ConstNodulePtr NodeGadget::nodule( Gaffer::ConstPlugPtr plug ) const
{
	return 0;
}
