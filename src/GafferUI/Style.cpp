#include "GafferUI/Style.h"
#include "GafferUI/StandardStyle.h"

#include "IECore/Exception.h"
#include "IECore/SimpleTypedData.h"

using namespace GafferUI;

IE_CORE_DEFINERUNTIMETYPED( Style );

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

const std::string &Style::stateAttribute()
{
	static std::string s = "user:GafferUI:state";
	return s;
}

IECore::ConstDataPtr Style::stateValueNormal()
{
	static IECore::DataPtr d = 0;
	if( !d )
	{
		d = new IECore::StringData( "normal" );
	}
	return d;
}

IECore::ConstDataPtr Style::stateValueInactive()
{
	static IECore::DataPtr d = 0;
	if( !d )
	{
		d = new IECore::StringData( "inactive" );
	}
	return d;
}

IECore::ConstDataPtr Style::stateValueSelected()
{
	static IECore::DataPtr d = 0;
	if( !d )
	{
		d = new IECore::StringData( "selected" );
	}
	return d;
}
