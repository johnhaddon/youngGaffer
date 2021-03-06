from __future__ import with_statement

from OpenGL.GL import *
import gtk
import IECore
import IECoreGL

import GafferUI
from GLWidget import GLWidget
from _GafferUI import ButtonEvent, ModifiableEvent, ContainerGadget, DragDropEvent, KeyEvent

## The GadgetWidget class provides a means of
# hosting a Gadget within a Widget based interface.
# Widgets are UI elements implemented using GTK, whereas
# Gadgets are implemented on top of the Cortex infrastructure.
#
# Camera motion is achieved by holding down either Ctrl or Alt and clicking
# and dragging. Use the left button for tumbling, the middle mouse button
# for tracking and the right mouse button for dollying. Prefer Ctrl rather than
# Alt on Apple's X11 with a single mouse button and 3 button emulation turned on.
#
## \todo It feels like this could be split into two classes - one that just
# takes gtk events and turns them into GafferUI events, and one that takes
# those events and forwards them to the Gadgets appropriately, maintaining
# current drag state etc. The latter class could then be used if implementing
# other hosts.
## \todo The camera movement should be coming from somewhere else - some kind
# of ViewportGadget or summink?
class GadgetWidget( GafferUI.GLWidget ) :

	CameraMode = IECore.Enum.create( "None", "Mode2D", "Mode3D" )

	def __init__( self, gadget=None, bufferOptions=set(), cameraMode=CameraMode.Mode2D ) :
		
		GLWidget.__init__( self, bufferOptions )
		
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
		self.gtkWidget().connect( "scroll_event", self.__scroll )

		self.__camera = IECore.Camera()
		self.__cameraController = IECore.CameraController( self.__camera )

		self.__renderer = IECoreGL.Renderer()
		self.__renderer.setOption( "gl:mode", IECore.StringData( "deferred" ) )
				
		self.setCameraMode( cameraMode )
		self.setBackgroundColor( IECore.Color3f( 0 ) )
		self.setGadget( gadget )
		
		self.__lastButtonPressGadget = None
		self.__dragDropEvent = None
		
		self.__cameraInMotion = False
		
		self.__baseState = IECoreGL.State( True )
		
	def setGadget( self, gadget ) :
	
		self.__gadget = gadget
		self.__scene = None
		
		if self.__gadget :
			self.__renderRequestConnection = self.__gadget.renderRequestSignal().connect( self.__renderRequest )
		else :
			self.__renderRequestConnection = None
			
		framingBound = self._framingBound()
		if not framingBound.isEmpty() :
			self.__cameraController.frame( framingBound )
		
	def getGadget( self ) :
	
		return self.__gadget
	
	def setCameraMode( self, cameraMode ) :
	
		self.__cameraMode = cameraMode
		if cameraMode==self.CameraMode.Mode2D :
			self.__camera.parameters()["projection"] = IECore.StringData( "orthographic" )
		else :
			self.__camera.parameters()["projection"] = IECore.StringData( "perspective" )
		
		# force the controller to update
		self.__cameraController.setCamera( self.__camera )
				
	def getCameraMode( self ) :
	
		return self.__cameraMode
	
	## \todo Should there be a standard way of setting colors for all Widgets?	
	def setBackgroundColor( self, color ) :
	
		self.__backgroundColor = color
	
	def getBackgroundColor( self ) :
	
		return self.__backgroundColor
	
	## Returns the bounding box which will be framed when "f" is pressed. This
	# may be overridden in derived classes to implement more intelligent framing.
	def _framingBound( self ) :
	
		if self.__gadget :
			return self.__gadget.bound()
		else :
			return IECore.Box3f()
			
	def _draw( self ) :
	
		bg = self.__backgroundColor.linearToSRGB()
		glClearColor( bg[0], bg[1], bg[2], 0.0 )
		glClearDepth( 1.0 )
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT )
			  
		self.__updateScene()
		if self.__scene :
			
			self.__scene.setCamera( IECoreGL.ToGLCameraConverter( self.__camera ).convert() )
			self.__scene.render( self.__baseState )
	
	## Returns the IECoreGL.State object used as the base display style for the
	# rendering of the held Gadget. This may be modified freely to
	# change the display style.
	def baseState( self ) :
	
		return self.__baseState
	
	def __renderRequest( self, gadget ) :
	
		# the gadget we're holding has requested to be rerendered.
		# destroy our scene so we'll have to rebuild it, and ask
		# that we're redrawn.
		self.__scene = None
		self.gtkWidget().queue_draw()
			
	def __updateScene( self ) :
	
		if not self.__gadget :
			self.__scene = None
			return
			
		if self.__scene :
			return
			
		with IECore.WorldBlock( self.__renderer ) :
		
			## \todo Should be able to rely on the IECoreGL::Renderer having a better default shader
			self.__renderer.shader( "surface", "constant", {} )
			self.__gadget.render( self.__renderer )

		self.__scene = self.__renderer.scene()

	def __buttonPress( self, widget, event ) :
			
		if not self.__gadget :
			return False

		gadgetEvent = self.__gtkEventToGadgetEvent( event )
		if not gadgetEvent.buttons :
			return False		

		if gadgetEvent.modifiers & ModifiableEvent.Modifiers.Alt or gadgetEvent.modifiers & ModifiableEvent.Modifiers.Control and not gadgetEvent.modifiers & ModifiableEvent.Modifiers.Shift :
			return self.__cameraButtonPress( event );

		gadgets = self.__select( event )
		gadget, result = self.__dispatchEvent( gadgets, "buttonPressSignal", gadgetEvent )
		if result :
			self.__lastButtonPressGadget = gadget
			return True
		else :
			self.__lastButtonPressGadget = None
			return False

	def __buttonRelease( self, widget, event ) :
	
		if not self.__gadget :
			return True

		if self.__cameraInMotion :
			return self.__cameraButtonRelease( event );
		
		gadgetEvent = self.__gtkEventToGadgetEvent( event )
		if not gadgetEvent.buttons :
			return True		

		gadgets = self.__select( event )
		if self.__dragDropEvent :
		
			self.__gtkEventToGadgetEvent( event, self.__dragDropEvent )
			dropGadget, result = self.__dispatchEvent( gadgets, "dropSignal", self.__dragDropEvent )
			self.__dragDropEvent.destination = dropGadget
			self.__dragDropEvent.dropResult = result
			self.__dispatchEvent( self.__dragDropEvent.source, "dragEndSignal", self.__dragDropEvent, dispatchToAncestors=False )
			self.__dragDropEvent = None
			
		elif self.__lastButtonPressGadget :
			
			self.__dispatchEvent( self.__lastButtonPressGadget, "buttonReleaseSignal", gadgetEvent, dispatchToAncestors=False )
			self.__lastButtonPressGadget = None
			
		return True

	def __motion( self, widget, event ) :
	
		if not self.__gadget :
			return True
		
		if self.__cameraInMotion :
			return self.__cameraMotion( event );

		if self.__lastButtonPressGadget and not self.__dragDropEvent :
			
			# try to start a new drag
			dragDropEvent = self.__gtkEventToGadgetEvent( event, DragDropEvent() )
			dragDropEvent.source = self.__lastButtonPressGadget
			g, d = self.__dispatchEvent( self.__lastButtonPressGadget, "dragBeginSignal", dragDropEvent, dispatchToAncestors=False )
			if d :
				dragDropEvent.data = d
				self.__dragDropEvent = dragDropEvent
				
			self.__lastButtonPressGadget = None
			
		elif self.__dragDropEvent :
		
			# update an existing drag
			
			self.__gtkEventToGadgetEvent( event, self.__dragDropEvent )			
			self.__dispatchEvent( self.__dragDropEvent.source, "dragUpdateSignal", self.__dragDropEvent, dispatchToAncestors=False )
		
		return True

	def __enterNotify( self, widget, event ) :
	
		if event.mode==gtk.gdk.CROSSING_NORMAL :
			# very important we only grab focus when its the mouse which
			# is entering the window. the other crossing modes are for gaining
			# or losing focus and grabbing focus in those seems to stop anyone
			# else from getting focus properly.
			widget.grab_focus()
		
	def __keyPress( self, widget, event ) :
	
		if not self.__gadget :
			return True
		
		# 'f' for framing	
		if event.keyval==102 :
			bound = self._framingBound()
			if not bound.isEmpty() :
				self.__cameraController.frame( bound )
				widget.queue_draw()
			return True
				
		# pass the key to the gadget
		gadgetEvent = self.__gtkEventToGadgetEvent( event )
		self.__dispatchEvent( self.getGadget(), "keyPressSignal", gadgetEvent )
		
		return True
	
	## Dispatches an event to a named signal on a Gadget, returning a tuple containing the
	# gadget that handled the event and the return value from the handler. A list of Gadgets
	# may be passed, in which case the event will be dispatched to them in order until it is
	# handled by one of them. If dispatchToAncestors is True, then the event will be despatched
	# to all the Gadgets from the root to the target Gadget, with the despatch terminating if
	# one of these ancestors handles the signal. Returns the gadget that handled the event and
	# the event result.
	def __dispatchEvent( self, gadgetOrGadgets, signalName, gadgetEvent, dispatchToAncestors=True, _leafGadget=None ) :
		
		## If it's a list of Gadgets call ourselves again for each element.		
		if isinstance( gadgetOrGadgets, list ) :
		
			for g in gadgetOrGadgets :
			
				handler, result = self.__dispatchEvent( g, signalName, gadgetEvent, dispatchToAncestors )
				if result :
					return handler, result
					
			return None, None
				
		## Or if we've been asked to despatch to the ancestors of a single gadget then do that.
		if dispatchToAncestors :
		
			gadget = gadgetOrGadgets
			
			## \todo It'd be nice to bind an ancestors() method so we get this list more easily. It might
			# be nice to implement custom iterators on the c++ side to make it easy to traverse the graph too - can
			# we use boost::graph stuff to help with that?
			ancestors = []
			a = gadget
			while a :
				ancestors.insert( 0, a )
				a = a.parent()
				
			for a in ancestors :
				
				handler, result = self.__dispatchEvent( a, signalName, gadgetEvent, dispatchToAncestors=False, _leafGadget=gadget )
				if result :
					return handler, result
					
			return None, None
		
		## Otherwise it's just a single Gadget to despatch directly to.
		
		gadget = gadgetOrGadgets
		
		if hasattr( gadgetEvent, "line" ) :
				
			# Transform into Gadget space
			untransformedLine = gadgetEvent.line
			m = gadget.fullTransform()
			m.invert( True )
			gadgetEvent.line *= m
		
		else :
		
			untransformedLine = None
				
		signal = getattr( gadget, signalName )()
		result = signal( _leafGadget or gadget, gadgetEvent )

		gadgetEvent.line = untransformedLine
		
		if result :
		
			return gadget, result
				
		return None, None

	## Returns a list of Gadgets under the screen x,y position specified by event.
	# The first Gadget in the list will be the frontmost, determined either by the
	# depth buffer if it exists or the drawing order if it doesn't.
	def __select( self, gtkEvent ) :
	
		if not self.__scene :
			return []
		
		drawable = self.gtkWidget().get_gl_drawable()
		context = self.gtkWidget().get_gl_context()
				
		if not drawable.gl_begin( context )  :
			return []
		
		viewportSize = IECore.V2f( self.gtkWidget().allocation.width, self.gtkWidget().allocation.height )
		regionCentre = IECore.V2f( gtkEvent.x, gtkEvent.y ) / viewportSize
		regionSize = IECore.V2f( 2 ) / viewportSize
		
		region = IECore.Box2f( regionCentre - regionSize/2, regionCentre + regionSize/2 )
		
		glClearColor( 0.0, 0.0, 0.0, 0.0 );
		glClearDepth( 1.0 )
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		selection = self.__scene.select( region )
		
		drawable.gl_end()

		if not len( selection ) :
			return []
		
		if context.get_gl_config().has_depth_buffer() :
			selection.sort()
		else :
			selection.reverse()
				
		result = []
		for s in selection :
			
			name = s.name.value()
			nameComponents = name.split( "." )
			g = self.__gadget
			assert( g.getName() == nameComponents[0] )
			for i in range( 1, len( nameComponents ) ) :
				g = g.getChild( nameComponents[i] )
				
			result.append( g )
				
		return result
	
	#########################################################################################################
	# stuff for controlling the camera
	#########################################################################################################
	
	def __configure( self, widget, event ) :
	
		self.__cameraController.setResolution( IECore.V2i( widget.allocation.width, widget.allocation.height ) )
		return False
	
	def __cameraButtonPress( self, event ) :
								
		motionType = IECore.CameraController.MotionType.None
		if event.button==1 :
			motionType = IECore.CameraController.MotionType.Tumble
		elif event.button==2 :
			motionType = IECore.CameraController.MotionType.Track
		elif event.button==3 :
			motionType = IECore.CameraController.MotionType.Dolly
				
		if motionType==IECore.CameraController.MotionType.Tumble and self.__cameraMode==self.CameraMode.Mode2D :
			motionType = IECore.CameraController.MotionType.Track
				
		if motionType :
			self.__cameraController.motionStart( motionType, IECore.V2i( int(event.x), int(event.y) ) )
			self.__cameraInMotion = True
			
		return True
	
	def __cameraMotion( self, event ) :
		
		self.__cameraController.motionUpdate( IECore.V2i( int(event.x), int(event.y) ) )
		self.gtkWidget().queue_draw()

		return True
		
	def __cameraButtonRelease( self, event ) :

		self.__cameraController.motionEnd( IECore.V2i( int(event.x), int(event.y) ) )
		self.gtkWidget().queue_draw()
		self.__cameraInMotion = False
		return True
		
	def __scroll( self, widget, event ) :
	
		if event.direction in (gtk.gdk.SCROLL_UP, gtk.gdk.SCROLL_DOWN) :
		
			position = IECore.V2i( int(event.x), int(event.y) )
			self.__cameraController.motionStart( IECore.CameraController.MotionType.Dolly, position )
			
			if event.direction==gtk.gdk.SCROLL_UP :
				position.x += 20
			else :
				position.x -= 20
			self.__cameraController.motionUpdate( position )
			
			self.__cameraController.motionEnd( position )
			self.gtkWidget().queue_draw()
			
	
	#########################################################################################################
	# conversion of gtk events to gadget events
	#########################################################################################################
	
	def __gtkEventToGadgetEvent( self, gtkEvent, gadgetEvent=None ) :
	
		if gadgetEvent is None :
		
			if gtkEvent.type==gtk.gdk.BUTTON_PRESS :
				gadgetEvent = ButtonEvent()
			elif gtkEvent.type==gtk.gdk.BUTTON_RELEASE :
				gadgetEvent = ButtonEvent()
			elif gtkEvent.type==gtk.gdk.MOTION_NOTIFY :
				gadgetEvent = ButtonEvent()
			elif gtkEvent.type==gtk.gdk.KEY_PRESS :
				gadgetEvent = KeyEvent()
			else :
				raise ValueError( "Unsupported event type " + str( gtkEvent.type ) )
		
		if isinstance( gadgetEvent, ModifiableEvent ) :
			gadgetEvent.modifiers = GadgetWidget.__gtkStateToEventModifiers( gtkEvent.state )
		
		if isinstance( gadgetEvent, ButtonEvent ) :
		
			if hasattr( gtkEvent, "button" ) :
				gadgetEvent.buttons = GadgetWidget.__gtkButtonToEventButton( gtkEvent.button )
			else :
				gadgetEvent.buttons = GadgetWidget.__gtkStateToEventButton( gtkEvent.state )
		
			if hasattr( gtkEvent, "x" ) :
			
				gadgetEvent.line.p0, gadgetEvent.line.p1 = self.__cameraController.unproject( IECore.V2i( int(gtkEvent.x), int(gtkEvent.y) ) )
		
		if isinstance( gadgetEvent, KeyEvent ) :
		
			gadgetEvent.key = gtk.gdk.keyval_name( gtkEvent.keyval ) 
		
		return gadgetEvent
		
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
