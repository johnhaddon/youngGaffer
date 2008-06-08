#ifndef GAFFERUI_NAMEGADGET_H
#define GAFFERUI_NAMEGADGET_H

#include "GafferUI/TextGadget.h"

#include "IECore/Font.h"

namespace GafferUI
{

class NameGadget : public TextGadget
{

	public :

		NameGadget( Gaffer::GraphComponentPtr object );
		virtual ~NameGadget();

		IE_CORE_DECLARERUNTIMETYPEDEXTENSION( NameGadget, NameGadgetTypeId, TextGadget );

	private :
	
		void nameChanged( Gaffer::GraphComponentPtr object );
		
};

IE_CORE_DECLAREPTR( NameGadget );

} // namespace GafferUI

#endif // GAFFERUI_NAMEGADGET_H
