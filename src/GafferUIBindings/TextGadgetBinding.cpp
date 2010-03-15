#include "boost/python.hpp"

#include "GafferUIBindings/TextGadgetBinding.h"
#include "GafferUI/TextGadget.h"

#include "IECorePython/RunTimeTypedBinding.h"

using namespace boost::python;
using namespace GafferUIBindings;
using namespace GafferUI;

void GafferUIBindings::bindTextGadget()
{
	IECorePython::RunTimeTypedClass<TextGadget>()
		.def( init<const std::string &>() )
		.def( "getFont", &TextGadget::getFont )
		.def( "setFont", &TextGadget::setFont )
		.def( "getText", &TextGadget::getText, return_value_policy<copy_const_reference>() )
		.def( "setText", &TextGadget::setText )
	;
}
