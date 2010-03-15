#include "boost/python.hpp"

#include "GafferUIBindings/GraphGadgetBinding.h"
#include "GafferUI/GraphGadget.h"
#include "GafferUI/NodeGadget.h"

#include "Gaffer/Node.h"

#include "IECorePython/RunTimeTypedBinding.h"

using namespace boost::python;
using namespace GafferUIBindings;
using namespace GafferUI;

void GafferUIBindings::bindGraphGadget()
{
	IECorePython::RunTimeTypedClass<GraphGadget>()
		.def( init<Gaffer::NodePtr>() )
		.def( "nodeGadget", (NodeGadgetPtr (GraphGadget::*)( Gaffer::ConstNodePtr ))&GraphGadget::nodeGadget )
	;
}
