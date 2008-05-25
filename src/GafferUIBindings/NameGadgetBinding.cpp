#include "boost/python.hpp"

#include "GafferUIBindings/NameGadgetBinding.h"
#include "GafferUI/NameGadget.h"

#include "IECore/bindings/IntrusivePtrPatch.h"
#include "IECore/bindings/RunTimeTypedBinding.h"

using namespace boost::python;
using namespace GafferUIBindings;
using namespace GafferUI;

void GafferUIBindings::bindNameGadget()
{
	typedef class_<NameGadget, NameGadgetPtr, boost::noncopyable, bases<TextGadget> > NameGadgetPyClass;

	NameGadgetPyClass( "NameGadget", init<IECore::FontPtr, Gaffer::GraphComponentPtr>() )
		.IE_COREPYTHON_DEFRUNTIMETYPEDSTATICMETHODS( NameGadget )
	;
		
	INTRUSIVE_PTR_PATCH( NameGadget, NameGadgetPyClass );
	
	implicitly_convertible<NameGadgetPtr, TextGadgetPtr>();
	implicitly_convertible<NameGadgetPtr, ConstNameGadgetPtr>();

}
