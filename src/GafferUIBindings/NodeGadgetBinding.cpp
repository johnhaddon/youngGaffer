#include "boost/python.hpp"

#include "GafferUIBindings/NodeGadgetBinding.h"
#include "GafferUI/NodeGadget.h"
#include "GafferUI/Nodule.h"

#include "IECorePython/RunTimeTypedBinding.h"

using namespace boost::python;
using namespace GafferUIBindings;
using namespace GafferUI;

void GafferUIBindings::bindNodeGadget()
{
	IECorePython::RunTimeTypedClass<NodeGadget>()
		.def( "node", (Gaffer::NodePtr (NodeGadget::*)())&NodeGadget::node )
		.def( "nodule", (NodulePtr (NodeGadget::*)( Gaffer::ConstPlugPtr ))&NodeGadget::nodule )
		.def( "create", &NodeGadget::create ).staticmethod( "create" )
	;
}
