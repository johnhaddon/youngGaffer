#ifndef GAFFERUI_GADGET_H
#define GAFFERUI_GADGET_H

#include "GafferUI/TypeIds.h"
#include "GafferUI/ButtonEvent.h"
#include "GafferUI/KeyEvent.h"

#include "Gaffer/GraphComponent.h"

#include "IECore/RunTimeTyped.h"
#include "IECore/Renderer.h"

#include "boost/signals.hpp"

namespace GafferUI
{

IE_CORE_FORWARDDECLARE( Gadget );

/// Gadgets are UI elements implemented on top of the Cortex project infrastructure - 
/// they draw themselves using the Renderer interface, and provide an interface for
/// handling events. They can therefore be used by any code willing to implement the
/// Renderer interface for real time display and provide suitable events - the GadgetWidget
/// python class is an example of such a host.
/// \todo Should there be some base class for this and the Widget class?
class Gadget : public Gaffer::GraphComponent
{

	public :

		Gadget( const std::string &name=staticTypeName() );
		virtual ~Gadget();

		IE_CORE_DECLARERUNTIMETYPEDEXTENSION( Gadget, GadgetTypeId, Gaffer::GraphComponent );

		/// @name Parent-child relationships
		////////////////////////////////////////////////////////////////////
		//@{
		/// By default Gadgets do not accept children. Derived classes which
		/// wish to act as containers for other Gadgets may reimplement this
		/// method to accept any children they wish.
		virtual bool acceptsChild( Gaffer::ConstGraphComponentPtr potentialChild ) const;
		/// Gadgets only accept other Gadgets as parent.
		virtual bool acceptsParent( const Gaffer::GraphComponent *potentialParent ) const;		
		//@}

		/// @name Display
		/// \todo I guess these could be implemented as signals too if we wanted.
		////////////////////////////////////////////////////////////////////
		//@{
		/// Renders the Gadget.
		void render( IECore::RendererPtr renderer ) const;
		/// The bounding box of the Gadget.
		virtual Imath::Box3f bound( IECore::RendererPtr renderer ) const = 0;
		//@}
		
		/// @name Events
		/// Events are specified as boost::signals. This allows anything to
		/// react to an event rather than just the Gadget receiving it,
		/// which makes for much more flexible customisation of the UI.
		////////////////////////////////////////////////////////////////////
		//@{
		/// A signal used to represent button related events.
		typedef boost::signal<bool ( GadgetPtr, const ButtonEvent &event )> ButtonSignal; 
		/// The signal triggered by a button press event.
		ButtonSignal &buttonPressSignal();
		/// The signal triggered by a button release event.
		ButtonSignal &buttonReleaseSignal();
		/// A signal used to represent key related events.
		typedef boost::signal<bool ( GadgetPtr, const KeyEvent &key )> KeySignal;
		/// The signal triggered by a key press event.
		KeySignal &keyPressSignal();
		/// The signal triggered by a key release event.
		KeySignal &keyReleaseSignal();
		//@}
		
	protected :
	
		/// The subclass specific part of render(). This must be implemented
		/// appropriately by all subclasses. The public render() method
		/// sets the renderer up with the name attribute for this Gadget and
		/// then calls doRender().
		virtual void doRender( IECore::RendererPtr renderer ) const = 0;
		
	private :
	
		Gadget *m_parent;
		
		ButtonSignal m_buttonPressSignal;
		ButtonSignal m_buttonReleaseSignal;

		KeySignal m_keyPressSignal;
		KeySignal m_keyReleaseSignal;

};

} // namespace GafferUI

#endif // GAFFERUI_GADGET_H
