#include "GafferBindings/ConnectionBinding.h"
#include "GafferBindings/SignalBinding.h"
#include "GafferBindings/GraphComponentBinding.h"

using namespace GafferBindings;

BOOST_PYTHON_MODULE( _Gaffer )
{

	bindConnection();
	bindGraphComponent();
	
	typedef boost::signal<int ( float )> TestSignal;
	bindSignal<TestSignal>( "TestSignal" );
	typedef boost::signal<float ( float, float )> TestSignal2;
	bindSignal<TestSignal2>( "TestSignal2" );

}
