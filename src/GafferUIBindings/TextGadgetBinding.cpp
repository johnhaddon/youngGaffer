#include "boost/python.hpp"

#include "GafferUIBindings/TextGadgetBinding.h"
#include "GafferUI/TextGadget.h"

#include "IECore/bindings/IntrusivePtrPatch.h"
#include "IECore/bindings/RunTimeTypedBinding.h"

using namespace boost::python;
using namespace GafferUIBindings;
using namespace GafferUI;

void GafferUIBindings::bindTextGadget()
{
	typedef class_<TextGadget, TextGadgetPtr, boost::noncopyable, bases<Gadget> > TextGadgetPyClass;

	TextGadgetPyClass( "TextGadget", init<const std::string &>() )
		.def( "getFont", &TextGadget::getFont )
		.def( "setFont", &TextGadget::setFont )
		.def( "getText", &TextGadget::getText, return_value_policy<copy_const_reference>() )
		.def( "setText", &TextGadget::setText )
		.IE_COREPYTHON_DEFRUNTIMETYPEDSTATICMETHODS( TextGadget )
	;
		
	INTRUSIVE_PTR_PATCH( TextGadget, TextGadgetPyClass );
	
	implicitly_convertible<TextGadgetPtr, GadgetPtr>();
	implicitly_convertible<TextGadgetPtr, ConstTextGadgetPtr>();

}
