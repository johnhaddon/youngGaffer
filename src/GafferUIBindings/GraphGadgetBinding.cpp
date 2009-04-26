#include "boost/python.hpp"

#include "GafferUIBindings/GraphGadgetBinding.h"
#include "GafferUI/GraphGadget.h"

#include "Gaffer/Node.h"

#include "IECore/bindings/RunTimeTypedBinding.h"

using namespace boost::python;
using namespace GafferUIBindings;
using namespace GafferUI;

void GafferUIBindings::bindGraphGadget()
{
	IECore::RunTimeTypedClass<GraphGadget>()
		.def( init<Gaffer::NodePtr>() )
	;
}
