#include "boost/python.hpp"

#include "GafferUIBindings/GraphGadgetBinding.h"
#include "GafferUI/GraphGadget.h"

#include "Gaffer/Node.h"

#include "IECore/bindings/IntrusivePtrPatch.h"
#include "IECore/bindings/RunTimeTypedBinding.h"

using namespace boost::python;
using namespace GafferUIBindings;
using namespace GafferUI;

void GafferUIBindings::bindGraphGadget()
{
	typedef class_<GraphGadget, GraphGadgetPtr, boost::noncopyable, bases<ContainerGadget> > GraphGadgetPyClass;

	GraphGadgetPyClass( "GraphGadget", init<Gaffer::NodePtr>() )
		.IE_COREPYTHON_DEFRUNTIMETYPEDSTATICMETHODS( GraphGadget )
	;
		
	INTRUSIVE_PTR_PATCH( GraphGadget, GraphGadgetPyClass );
	
	implicitly_convertible<GraphGadgetPtr, ContainerGadgetPtr>();
	implicitly_convertible<GraphGadgetPtr, ConstGraphGadgetPtr>();

}
