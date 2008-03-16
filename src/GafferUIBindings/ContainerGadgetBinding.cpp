#include "boost/python.hpp"

#include "GafferUIBindings/ContainerGadgetBinding.h"
#include "GafferUI/ContainerGadget.h"

#include "IECore/bindings/IntrusivePtrPatch.h"
#include "IECore/bindings/RunTimeTypedBinding.h"

using namespace boost::python;
using namespace GafferUIBindings;
using namespace GafferUI;

void GafferUIBindings::bindContainerGadget()
{
	typedef class_<ContainerGadget, ContainerGadgetPtr, boost::noncopyable, bases<Gadget> > ContainerGadgetPyClass;

	ContainerGadgetPyClass( "ContainerGadget", no_init )
		.def( "childTransform", &ContainerGadget::childTransform )
		.IE_COREPYTHON_DEFRUNTIMETYPEDSTATICMETHODS( ContainerGadget )
	;
		
	INTRUSIVE_PTR_PATCH( ContainerGadget, ContainerGadgetPyClass );
	
	implicitly_convertible<ContainerGadgetPtr, GadgetPtr>();
	implicitly_convertible<ContainerGadgetPtr, ConstContainerGadgetPtr>();

}
