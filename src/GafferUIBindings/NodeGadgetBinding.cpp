#include "boost/python.hpp"

#include "GafferUIBindings/NodeGadgetBinding.h"
#include "GafferUI/NodeGadget.h"
#include "GafferUI/Nodule.h"

#include "IECore/bindings/RunTimeTypedBinding.h"

using namespace boost::python;
using namespace GafferUIBindings;
using namespace GafferUI;

void GafferUIBindings::bindNodeGadget()
{
	IECore::RunTimeTypedClass<NodeGadget>()
		.def( init<Gaffer::NodePtr>() )
		.def( "node", (Gaffer::NodePtr (NodeGadget::*)())&NodeGadget::node )
		.def( "nodule", (NodulePtr (NodeGadget::*)( Gaffer::ConstPlugPtr ))&NodeGadget::nodule )
	;
}
