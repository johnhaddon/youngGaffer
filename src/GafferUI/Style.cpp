#include "GafferUI/Style.h"
#include "GafferUI/StandardStyle.h"

using namespace GafferUI;

StylePtr Style::g_defaultStyle = new StandardStyle;

Style::Style()
{
}

Style::~Style()
{
}

StylePtr Style::getDefaultStyle()
{
	return g_defaultStyle;
}

void Style::setDefaultStyle( StylePtr style )
{
	g_defaultStyle = style;
}
