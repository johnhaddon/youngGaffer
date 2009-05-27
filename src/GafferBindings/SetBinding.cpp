#include "GafferBindings/SetBinding.h"

namespace GafferBindings
{

void bindSet()
{
	bindSet<Gaffer::NodeSet>();
	bindSet<Gaffer::GraphComponentSet>();
}

}
