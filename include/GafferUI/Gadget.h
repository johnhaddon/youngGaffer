#ifndef GAFFERUI_GADGET_H
#define GAFFERUI_GADGET_H

#include "GafferUI/TypeIds.h"
#include "GafferUI/ButtonEvent.h"
#include "GafferUI/KeyEvent.h"
#include "GafferUI/EventSignalCombiner.h"

#include "Gaffer/GraphComponent.h"

#include "IECore/RunTimeTyped.h"
#include "IECore/Renderer.h"

#include "boost/signals.hpp"

namespace GafferUI
{

IE_CORE_FORWARDDECLARE( Gadget );
IE_CORE_FORWARDDECLARE( Style );

/// Gadgets are UI elements implemented on top of the Cortex project infrastructure - 
/// they draw themselves using the Renderer interface, and provide an interface for
/// handling events. They can therefore be used by any code willing to implement the
/// Renderer interface for real time display and provide suitable events - the GadgetWidget
/// python class is an example of such a host.
/// \todo Should there be some base class for this and the Widget class?
///
/// BASIC PLAN :
///
///	* The Gadget passed as the argument to signals is the leaf gadget that received the event.
/// * But events are passed first to the topmost parent of that leaf - and then down the
///   hierarchy till the child is reached.
/// * If any handler returns true then the entire traversal is cut short there.
/// * It is the responsibility of the host (GadgetWidget) to perform this traversal.
/// * ContainerGadget is a base class for all Gadgets which have children. It has a
///   virtual method to return the transform for any given child? It is the responsibility
///   of the host to use this transform to convert the coordinates in the event into the
///   widgets own object space before calling the event? This is my least favourite bit of this
///   scheme, but otherwise the container widget is responsible for passing events on itself, and
///   it gets slightly ugly.
///
class Gadget : public Gaffer::GraphComponent
{

	public :

		Gadget( const std::string &name=staticTypeName() );
		virtual ~Gadget();

		IE_CORE_DECLARERUNTIMETYPEDEXTENSION( Gadget, GadgetTypeId, Gaffer::GraphComponent );

		/// @name Parent-child relationships
		////////////////////////////////////////////////////////////////////
		//@{
		/// By default Gadgets do not accept children. Derive from ContainerGadget
		/// if you wish to accept children.
		virtual bool acceptsChild( Gaffer::ConstGraphComponentPtr potentialChild ) const;
		/// Gadgets only accept other Gadgets as parent.
		virtual bool acceptsParent( const Gaffer::GraphComponent *potentialParent ) const;		
		//@}

		/// @name Style
		/// Every Gadget has a Style object to define its appearance. This is set
		/// to the result of Style::getDefaultStyle() on construction but can be
		/// subsequently changed if necessary.
		////////////////////////////////////////////////////////////////////
		//{@
		ConstStylePtr getStyle() const;
		void setStyle( ConstStylePtr style );
		//@}

		/// @name Display
		/// \todo I guess these could be implemented as signals too if we wanted.
		////////////////////////////////////////////////////////////////////
		//@{
		/// Renders the Gadget.
		void render( IECore::RendererPtr renderer ) const;
		/// The bounding box of the Gadget.
		virtual Imath::Box3f bound() const = 0;
		typedef boost::signal<void ( GadgetPtr )> RenderRequestSignal;
		RenderRequestSignal &renderRequestSignal();
		//@}
		
		/// @name Events
		/// Events are specified as boost::signals. This allows anything to
		/// react to an event rather than just the Gadget receiving it,
		/// which makes for much more flexible customisation of the UI.
		////////////////////////////////////////////////////////////////////
		//@{
		/// A signal used to represent button related events.
		typedef boost::signal<bool ( GadgetPtr, const ButtonEvent &event ), EventSignalCombiner> ButtonSignal; 
		/// The signal triggered by a button press event.
		ButtonSignal &buttonPressSignal();
		/// The signal triggered by a button release event.
		ButtonSignal &buttonReleaseSignal();
		/// A signal used to represent key related events.
		/// \todo We need some sort of focus model to say who gets the events.
		typedef boost::signal<bool ( GadgetPtr, const KeyEvent &key ), EventSignalCombiner> KeySignal;
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
		
		Gadget();
		
		ConstStylePtr m_style;
		
		RenderRequestSignal m_renderRequestSignal;
			
		ButtonSignal m_buttonPressSignal;
		ButtonSignal m_buttonReleaseSignal;

		KeySignal m_keyPressSignal;
		KeySignal m_keyReleaseSignal;

};

} // namespace GafferUI

#endif // GAFFERUI_GADGET_H
