#ifndef GAFFER_COMPOUNDPLUG_H
#define GAFFER_COMPOUNDPLUG_H

#include "Gaffer/ValuePlug.h"

namespace Gaffer
{

class CompoundPlug : public ValuePlug
{

	public :
			
		CompoundPlug( const std::string &name=staticTypeName(), Direction direction=In, unsigned flags=None );
		virtual ~CompoundPlug();

		IE_CORE_DECLARERUNTIMETYPEDEXTENSION( CompoundPlug, CompoundPlugTypeId, ValuePlug );

		/// Accepts any child provided it's a Plug and has the same direction
		/// as this CompoundPlug.
		virtual bool acceptsChild( ConstGraphComponentPtr potentialChild ) const;
				
		/// Only accepts inputs which are CompoundPlugs with child
		/// Plugs compatible with this plug.
		virtual bool acceptsInput( ConstPlugPtr input ) const;
		/// Makes connections between the corresponding child Plugs of
		/// input and this Plug.
		virtual void setInput( PlugPtr input );

		/// Sets all child plugs dirty.
		virtual void setDirty();

	protected :

		virtual void setFromInput();
		
	private :
	
		void parentChanged();
		void childAddedOrRemoved();
	
		boost::signals::connection m_plugInputChangedConnection;
		void plugInputChanged( PlugPtr plug );
		
		void updateInputFromChildInputs( PlugPtr checkFirst );
		
};

IE_CORE_DECLAREPTR( CompoundPlug );

} // namespace Gaffer

#endif // GAFFER_COMPOUNDPLUG_H
