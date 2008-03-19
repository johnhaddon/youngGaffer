from GLWidget import GLWidget
from _GafferUI import ButtonEvent, ModifiableEvent
from OpenGL.GL import *
from IECoreGL import Renderer
import IECore
import gtk

## The GadgetWidget class provides a means of
# hosting a Gadget within a Widget based interface.
# Widgets are UI elements implemented using GTK, whereas
# Gadgets are implemented on top of the Cortex infrastructure.
class GadgetWidget( GLWidget ) :

	def __init__( self, gadget=None ) :
		
		GLWidget.__init__( self )
		
		self.gtkWidget().add_events( gtk.gdk.BUTTON_PRESS_MASK )
		self.gtkWidget().add_events( gtk.gdk.BUTTON_RELEASE_MASK )
		self.gtkWidget().add_events( gtk.gdk.POINTER_MOTION_MASK )
		self.gtkWidget().add_events( gtk.gdk.KEY_PRESS_MASK )
		self.gtkWidget().add_events( gtk.gdk.KEY_RELEASE_MASK )
		self.gtkWidget().add_events( gtk.gdk.ENTER_NOTIFY_MASK )
		self.gtkWidget().set_flags( gtk.CAN_FOCUS )
				
		self.gtkWidget().connect( "button_press_event", self.__buttonPress )
		self.gtkWidget().connect( "button_release_event", self.__buttonRelease )
		self.gtkWidget().connect( "motion_notify_event", self.__motion )
		self.gtkWidget().connect( "key_press_event", self.__keyPress )
		self.gtkWidget().connect( "configure_event", self.__configure )
		self.gtkWidget().connect( "enter_notify_event", self.__enterNotify )

		self.__camera = IECore.Camera()
		self.__camera.parameters()["projection"] = IECore.StringData( "perspective" )
		self.__cameraController = IECore.CameraController( self.__camera )
		
		self.setGadget( gadget )
		
	def setGadget( self, gadget ) :
	
		self.__gadget = gadget
		self.__scene = None
		self.__cameraController.frame( self.__gadget.bound( None ) )
		
	def getGadget( self ) :
	
		return self.__gadget
		
	def draw( self ) :
	
		glClearColor( 0.0, 0.0, 0.0, 0.0 )
		glClearDepth( 1.0 )
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT )
			  
		self.__updateScene()
		if self.__scene :
			
			self.__scene.render()
			
	def __updateScene( self ) :
	
		if not self.__gadget :
			self.__scene = None
			return

		renderer = Renderer()
		renderer.setOption( "gl:mode", IECore.StringData( "deferred" ) )
		self.__camera.render( renderer )
				
		renderer.worldBegin()
		if 1 :
			
			renderer.concatTransform( IECore.M44f.createTranslated( IECore.V3f( 0, 0, -1 ) ) )			
			renderer.setAttribute( "gl:primitive:wireframe", IECore.BoolData( 1 ) )
			self.__gadget.render( renderer )

		renderer.worldEnd()
		
		self.__scene = renderer.scene()

	def __buttonPress( self, widget, event ) :
			
		if not self.__gadget :
			return False

		gadgetEvent = self.__gtkEventToGadgetEvent( event )
		if not gadgetEvent.buttons :
			return False		

		if gadgetEvent.modifiers & ModifiableEvent.Modifiers.Alt :
			return self.__cameraButtonPress( event );

		gadgets = self.__select( event )
		for gadget in gadgets :
			
			handled = gadget.buttonPressSignal()( gadgets[-1], gadgetEvent )
			print gadget, handled
			if handled :
				return True
		
		return False

	def __buttonRelease( self, widget, event ) :
	
		if not self.__gadget :
			return True
		
		gadgetEvent = self.__gtkEventToGadgetEvent( event )
		if not gadgetEvent.buttons :
			return True		

		if gadgetEvent.modifiers & ModifiableEvent.Modifiers.Alt :
			return self.__cameraButtonRelease( event );

		gadgets = self.__select( event )
		return True

	def __motion( self, widget, event ) :
	
		if not self.__gadget :
			return True
		
		gadgetEvent = self.__gtkEventToGadgetEvent( event )
		if gadgetEvent.modifiers & ModifiableEvent.Modifiers.Alt :
			return self.__cameraMotion( event );

		gadgets = self.__select( event )
		return True

	def __enterNotify( self, widget, event ) :
	
		widget.grab_focus()
		return True
		
	def __keyPress( self, widget, event ) :
	
		if not self.__gadget :
			return True
			
		if event.keyval==102 :
			self.__cameraController.frame( self.__gadget.bound( None ) )
			widget.queue_draw()
			return True
		
		return True
		
	def __dispatchEvent( self, gadgets, signalName, gadgetEvent ) :
	
		for i in range( 0, len( gadgets ) ) :
		
			if i > 0 :
				if hasattr( gadgetEvent, "rayOrigin" ) :
					m = gadgets[i-1].childTransform( gadgets[i] )
					m.invert()
					gadgetEvent.rayOrigin *= m
					gadgetEvent.rayDirection = m.multDirMatrix( gadgetEvent.rayDirection )
					gadgetEvent.rayDirection = gadgetEvent.rayDirection.normalized()
		
			signal = getattr( gadgets[i], signalName )()
			
			handled = signal( gadgets[-1], gadgetEvent )
			print gadget, handled
			if handled :
				return handled
				
		return False

	# Returns a list containing the gadget hierarchy under event.x, event.y.
	# The top level parent is the first element in the list and the leaf gadget
	# is the last element.
	def __select( self, event ) :
	
		if not self.__scene :
			return None
		
		drawable = self.gtkWidget().get_gl_drawable()
		context = self.gtkWidget().get_gl_context()
		
		if not drawable.gl_begin( context )  :
			return []
		
		viewportSize = IECore.V2f( self.gtkWidget().allocation.width, self.gtkWidget().allocation.height )
		regionCentre = IECore.V2f( event.x, event.y ) / viewportSize
		regionSize = IECore.V2f( 10 ) / viewportSize
		
		region = IECore.Box2f( regionCentre - regionSize/2, regionCentre + regionSize/2 )
		
		glClearColor( 0.0, 0.0, 0.0, 0.0 );
		glClearDepth( 1.0 )
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		selection = self.__scene.select( region )
		
		drawable.gl_end()

		if not len( selection ) :
			return []
			
		selection.sort()
		name = selection[0].name.value()
		nameComponents = name.split( "." )
		result = [ self.__gadget ]
		assert( result[0].getName() == nameComponents[0] )
		for i in range( 1, len( nameComponents ) ) :
			result.append( result[-1].getChild( nameComponents[i] ) )
		
		return result
	
	#########################################################################################################
	# stuff for controlling the camera
	#########################################################################################################
	
	def __configure( self, widget, event ) :
	
		self.__cameraController.setResolution( IECore.V2i( widget.allocation.width, widget.allocation.height ) )
		return False
	
	def __cameraButtonPress( self, event ) :
				
		if event.button==1 :
			self.__cameraController.motionStart( IECore.CameraController.MotionType.Track, IECore.V2i( int(event.x), int(event.y) ) )
		elif event.button==2 :
			self.__cameraController.motionStart( IECore.CameraController.MotionType.Dolly, IECore.V2i( int(event.x), int(event.y) ) )
			
		return True
	
	def __cameraMotion( self, event ) :
	
		if not event.state & ( gtk.gdk.BUTTON1_MASK | gtk.gdk.BUTTON2_MASK | gtk.gdk.BUTTON3_MASK ) :
			return True
	
		self.__cameraController.motionUpdate( IECore.V2i( int(event.x), int(event.y) ) )
		self.gtkWidget().queue_draw()

		return True
		
	def __cameraButtonRelease( self, event ) :

		self.__cameraController.motionEnd( IECore.V2i( int(event.x), int(event.y) ) )
		self.gtkWidget().queue_draw()	
		return True
	
	#########################################################################################################
	# conversion of gtk events to gadget events
	#########################################################################################################
	
	def __gtkEventToGadgetEvent( self, gtkEvent ) :
	
		if gtkEvent.type==gtk.gdk.BUTTON_PRESS :
			event = ButtonEvent()
		elif gtkEvent.type==gtk.gdk.BUTTON_RELEASE :
			event = ButtonEvent()
		elif gtkEvent.type==gtk.gdk.MOTION_NOTIFY :
			event = ButtonEvent()
		else :
			raise ValueError( "Unsupported event type" )	
		
		if isinstance( event, ModifiableEvent ) :
			event.modifiers = GadgetWidget.__gtkStateToEventModifiers( gtkEvent.state )
		
		if isinstance( event, ButtonEvent ) :
		
			if hasattr( gtkEvent, "button" ) :
				event.buttons = GadgetWidget.__gtkButtonToEventButton( gtkEvent.button )
			else :
				event.buttons = GadgetWidget.__gtkStateToEventButton( gtkEvent.state )
		
			if hasattr( gtkEvent, "x" ) :
			
				event.rayOrigin, event.rayDirection = self.__cameraController.unproject( IECore.V2i( int(gtkEvent.x), int(gtkEvent.y) ) )
				event.rayDirection = ( event.rayDirection - event.rayOrigin ).normalized()
		
		return event
		
	@staticmethod
	def __gtkStateToEventModifiers( gtkState ) :
				
		result = ModifiableEvent.Modifiers.None
		if gtkState & gtk.gdk.CONTROL_MASK :
			result = result | ModifiableEvent.Modifiers.Control
			
		if gtkState & gtk.gdk.SHIFT_MASK :
			result = result | ModifiableEvent.Modifiers.Shift
		
		if gtkState & gtk.gdk.MOD1_MASK :
			result = result | ModifiableEvent.Modifiers.Alt
				
		return ModifiableEvent.Modifiers( result )

	@staticmethod
	def __gtkStateToEventButton( gtkState ) :
		
		result = ButtonEvent.Buttons.None
		if gtkState & gtk.gdk.BUTTON1_MASK :
			result = result | ButtonEvent.Buttons.Left
			
		if gtkState & gtk.gdk.BUTTON2_MASK :
			result = result | ButtonEvent.Buttons.Middle
		
		if gtkState & gtk.gdk.BUTTON3_MASK :
			result = result | ButtonEvent.Buttons.Right
				
		return ButtonEvent.Buttons( result )
		
	@staticmethod
	def __gtkButtonToEventButton( gtkButton ) :
	
		if gtkButton==1 :
			return ButtonEvent.Buttons.Left
		elif gtkButton==2 :
			return ButtonEvent.Buttons.Middle
		elif gtkButton==3 :
			return ButtonEvent.Buttons.Right
		else :
			return ButtonEvent.Buttons.None