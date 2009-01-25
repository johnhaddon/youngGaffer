#include "GafferBindings/ConnectionBinding.h"
#include "GafferBindings/SignalBinding.h"
#include "GafferBindings/GraphComponentBinding.h"
#include "GafferBindings/NodeBinding.h"
#include "GafferBindings/PlugBinding.h"
#include "GafferBindings/ValuePlugBinding.h"
#include "GafferBindings/NumericPlugBinding.h"
#include "GafferBindings/TypedPlugBinding.h"
#include "GafferBindings/ScriptNodeBinding.h"
#include "GafferBindings/ApplicationRootBinding.h"
#include "GafferBindings/SetBinding.h"
#include "GafferBindings/ObjectSignalBinding.h"
#include "GafferBindings/UndoContextBinding.h"

using namespace GafferBindings;

BOOST_PYTHON_MODULE( _Gaffer )
{

	bindConnection();
	bindGraphComponent();
	bindNode();
	bindPlug();
	bindValuePlug();
	bindNumericPlug();
	bindTypedPlug();
	bindScriptNode();
	bindApplicationRoot();
	bindSet();
	bindObjectSignal();
	bindUndoContext();
	
	typedef boost::signal<int ( float )> TestSignal;
	SignalBinder<TestSignal>::bind( "TestSignal" );
	typedef boost::signal<float ( float, float )> TestSignal2;
	SignalBinder<TestSignal2>::bind( "TestSignal2" );

}
