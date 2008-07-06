#include "boost/python.hpp"

#include "GafferUIBindings/NoduleBinding.h"
#include "GafferUI/Nodule.h"

#include "Gaffer/Plug.h"

#include "IECore/bindings/IntrusivePtrPatch.h"
#include "IECore/bindings/RunTimeTypedBinding.h"

using namespace boost::python;
using namespace GafferUIBindings;
using namespace GafferUI;

void GafferUIBindings::bindNodule()
{
	typedef class_<Nodule, NodulePtr, boost::noncopyable, bases<Gadget> > NodulePyClass;

	NodulePyClass( "Nodule", init<Gaffer::PlugPtr>() )
		.IE_COREPYTHON_DEFRUNTIMETYPEDSTATICMETHODS( Nodule )
	;
		
	INTRUSIVE_PTR_PATCH( Nodule, NodulePyClass );
	
	implicitly_convertible<NodulePtr, GadgetPtr>();
	implicitly_convertible<NodulePtr, ConstNodulePtr>();

}
