from GLWidget import GLWidget
from _GafferUI import ButtonEvent, ModifiableEvent, ContainerGadget, DragDropEvent
from OpenGL.GL import *
import IECore
import IECoreGL
import gtk

## The GadgetWidget class provides a means of
# hosting a Gadget within a Widget based interface.
# Widgets are UI elements implemented using GTK, whereas
# Gadgets are implemented on top of the Cortex infrastructure.
class GadgetWidget( GLWidget ) :

	CameraMode = IECore.Enum.create( "None", "Mode2D", "Mode3D" )

	def __init__( self, gadget=None, cameraMode=CameraMode.Mode2D ) :
		
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
		self.gtkWidget().connect( "scroll_event", self.__scroll )

		self.__camera = IECore.Camera()
		self.__cameraController = IECore.CameraController( self.__camera )

		self.__renderer = IECoreGL.Renderer()
		self.__renderer.setOption( "gl:mode", IECore.StringData( "deferred" ) )
				
		self.setCameraMode( cameraMode )
		self.setGadget( gadget )
		
		self.__lastButtonPressGadgets = None
		self.__dragSourceGadget = None
		self.__dragDropEvent = None
		
	def setGadget( self, gadget ) :
	
		self.__gadget = gadget
		self.__scene = None
		self.__renderRequestConnection = gadget.renderRequestSignal().connect( self.__renderRequest )
		
		bound = self.__gadget.bound()
		if not bound.isEmpty() :
			self.__cameraController.frame( bound )
		
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
	
	def draw( self ) :
	
		glClearColor( 0.0, 0.0, 0.0, 0.0 )
		glClearDepth( 1.0 )
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT )
			  
		self.__updateScene()
		if self.__scene :
			
			self.__scene.setCamera( IECoreGL.ToGLCameraConverter( self.__camera ).convert() )
			self.__scene.render()
	
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
			
		self.__renderer.worldBegin()
		if 1 :
			
			self.__renderer.shader( "surface", "constant", {} )
			self.__gadget.render( self.__renderer )

		self.__renderer.worldEnd()
		
		self.__scene = self.__renderer.scene()

	def __buttonPress( self, widget, event ) :
			
		if not self.__gadget :
			return False

		gadgetEvent = self.__gtkEventToGadgetEvent( event )
		if not gadgetEvent.buttons :
			return False		

		if gadgetEvent.modifiers & ModifiableEvent.Modifiers.Alt :
			return self.__cameraButtonPress( event );

		gadgets = self.__select( event )
		self.__lastButtonPressGadgets = gadgets
		gadget, result = self.__dispatchEvent( gadgets, "buttonPressSignal", gadgetEvent )
		if result :
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
		self.__lastButtonPressGadgets = False
		if self.__dragDropEvent :
			## \todo Send some kind of dragEnd/drop signal
			dropEvent = self.__gtkEventToGadgetEvent( event, self.__dragDropEvent )
			self.__dispatchEvent( gadgets, "dropSignal", self.__dragDropEvent )
			self.__dragSourceGadget = False
			self.__dragDropEvent = None
			
		return True

	def __motion( self, widget, event ) :
	
		if not self.__gadget :
			return True
		
		buttonEvent = self.__gtkEventToGadgetEvent( event, ButtonEvent() )
		if buttonEvent.modifiers & ModifiableEvent.Modifiers.Alt :
			return self.__cameraMotion( event );

		if self.__lastButtonPressGadgets and not self.__dragSourceGadget :
			# try to start a new drag
			dragDropEvent = self.__gtkEventToGadgetEvent( event, DragDropEvent() )
			g, d = self.__dispatchEvent( self.__lastButtonPressGadgets, "dragBeginSignal", dragDropEvent )
			if d :
				dragDropEvent.data = d
				self.__dragSourceGadget = g
				self.__dragDropEvent = dragDropEvent
				
			self.__lastButtonPressGadgets = None
		elif self.__dragSourceGadget :
			# update an existing drag
			self.__gtkEventToGadgetEvent( event, self.__dragDropEvent )
			self.__dragSourceGadget.dragUpdateSignal()( self.__dragSourceGadget, self.__dragDropEvent )
		
		return True

	def __enterNotify( self, widget, event ) :
	
		widget.grab_focus()
		return True
		
	def __keyPress( self, widget, event ) :
	
		if not self.__gadget :
			return True
			
		if event.keyval==102 :
			bound = self.__gadget.bound()
			if not bound.isEmpty() :
				self.__cameraController.frame( self.__gadget.bound() )
				widget.queue_draw()
			return True
		
		return True
		
	def __dispatchEvent( self, gadgets, signalName, gadgetEvent ) :
		
		for i in range( 0, len( gadgets ) ) :
				
			if i > 0 :
				parent = gadgets[i-1]
				if hasattr( gadgetEvent, "line" ) and parent.isInstanceOf( ContainerGadget.staticTypeId() ) :
					m = parent.childTransform( gadgets[i] )
					m.invert( True )
					gadgetEvent.line *= m
			
			
			print "SIGNAL", gadgets[i], signalName		
			signal = getattr( gadgets[i], signalName )()

			result = signal( gadgets[-1], gadgetEvent )
			if result :
				return gadgets[i], result
				
		return None, None

	# Returns a list containing the gadget hierarchy under event.x, event.y.
	# The top level parent is the first element in the list and the leaf gadget
	# is the last element.
	def __select( self, event ) :
	
		if not self.__scene :
			return []
		
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
		
		if context.get_gl_config().has_depth_buffer() :
			selection.sort()
			name = selection[0].name.value()
		else :
			name = selection[-1].name.value()
			
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
				
		motionType = IECore.CameraController.MotionType.None
		if event.button==1 :
			if event.state & gtk.gdk.CONTROL_MASK :
				motionType = IECore.CameraController.MotionType.Dolly
			elif event.state & gtk.gdk.SHIFT_MASK :
				motionType = IECore.CameraController.MotionType.Track
			elif event.state & gtk.gdk.BUTTON2_MASK :
				motionType = IECore.CameraController.MotionType.Dolly
			else :
				motionType = IECore.CameraController.MotionType.Tumble
		elif event.button==2 :
			if event.state & gtk.gdk.BUTTON1_MASK :
				motionType = IECore.CameraController.MotionType.Dolly
			else :
				motionType = IECore.CameraController.MotionType.Track
				
		if motionType==IECore.CameraController.MotionType.Tumble and self.__cameraMode==self.CameraMode.Mode2D :
			motionType = IECore.CameraController.MotionType.Track
				
		if motionType :
			self.__cameraController.motionStart( motionType, IECore.V2i( int(event.x), int(event.y) ) )
			
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
			else :
				raise ValueError( "Unsupported event type" )
		
		if isinstance( gadgetEvent, ModifiableEvent ) :
			gadgetEvent.modifiers = GadgetWidget.__gtkStateToEventModifiers( gtkEvent.state )
		
		if isinstance( gadgetEvent, ButtonEvent ) :
		
			if hasattr( gtkEvent, "button" ) :
				gadgetEvent.buttons = GadgetWidget.__gtkButtonToEventButton( gtkEvent.button )
			else :
				gadgetEvent.buttons = GadgetWidget.__gtkStateToEventButton( gtkEvent.state )
		
			if hasattr( gtkEvent, "x" ) :
			
				gadgetEvent.line.p0, gadgetEvent.line.p1 = self.__cameraController.unproject( IECore.V2i( int(gtkEvent.x), int(gtkEvent.y) ) )
		
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
