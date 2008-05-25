#include "boost/python.hpp"

#include "GafferUIBindings/GadgetBinding.h"
#include "GafferUIBindings/EventBinding.h"
#include "GafferUIBindings/ModifiableEventBinding.h"
#include "GafferUIBindings/KeyEventBinding.h"
#include "GafferUIBindings/ButtonEventBinding.h"
#include "GafferUIBindings/NodeGadgetBinding.h"
#include "GafferUIBindings/ContainerGadgetBinding.h"
#include "GafferUIBindings/GraphGadgetBinding.h"
#include "GafferUIBindings/RenderableGadgetBinding.h"
#include "GafferUIBindings/IndividualContainerBinding.h"
#include "GafferUIBindings/FrameBinding.h"
#include "GafferUIBindings/TextGadgetBinding.h"
#include "GafferUIBindings/NameGadgetBinding.h"

using namespace GafferUIBindings;

BOOST_PYTHON_MODULE( _GafferUI )
{

	bindGadget();
	bindEvent();
	bindModifiableEvent();
	bindKeyEvent();
	bindButtonEvent();
	bindNodeGadget();
	bindContainerGadget();
	bindGraphGadget();
	bindRenderableGadget();
	bindIndividualContainer();
	bindFrame();
	bindTextGadget();
	bindNameGadget();

}
