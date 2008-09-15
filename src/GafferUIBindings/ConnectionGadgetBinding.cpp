#include "boost/python.hpp"

#include "GafferUIBindings/ConnectionGadgetBinding.h"
#include "GafferUI/ConnectionGadget.h"
#include "GafferUI/Nodule.h"

#include "Gaffer/Node.h"

#include "IECore/bindings/IntrusivePtrPatch.h"
#include "IECore/bindings/RunTimeTypedBinding.h"

using namespace boost::python;
using namespace GafferUIBindings;
using namespace GafferUI;

void GafferUIBindings::bindConnectionGadget()
{
	typedef class_<ConnectionGadget, ConnectionGadgetPtr, boost::noncopyable, bases<Gadget> > ConnectionGadgetPyClass;

	ConnectionGadgetPyClass( "ConnectionGadget", init<GafferUI::NodulePtr, GafferUI::NodulePtr>() )
		.IE_COREPYTHON_DEFRUNTIMETYPEDSTATICMETHODS( ConnectionGadget )
	;
		
	INTRUSIVE_PTR_PATCH( ConnectionGadget, ConnectionGadgetPyClass );
	
	implicitly_convertible<ConnectionGadgetPtr, GadgetPtr>();
	implicitly_convertible<ConnectionGadgetPtr, ConstConnectionGadgetPtr>();

}
