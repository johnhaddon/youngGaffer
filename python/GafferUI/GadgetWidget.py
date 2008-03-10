from GLWidget import GLWidget
from _GafferUI import ButtonEvent
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
		self.gtkWidget().connect( "button_press_event", self.__buttonPress )
		self.gtkWidget().connect( "button_release_event", self.__buttonRelease )
		
		self.__gadget = gadget
		self.__scene = None
		
	def setGadget( self, gadget ) :
	
		self.__gadget = gadget
		
	def getGadget( self ) :
	
		return self.__gadget
		
	def draw( self ) :
	
		## \todo This camera stuff should be performed in the Scene, and specified by
		# some sort of ViewportGadget
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glColor3f(1.0, 1.0, 1.0);
		glMatrixMode( GL_PROJECTION )
		glLoadIdentity()
		glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0); 
			  
		self.__updateScene()
		if self.__scene :
			
			self.__scene.render()
			
	def __updateScene( self ) :
	
		if not self.__gadget :
			self.__scene = None
			return

		renderer = Renderer()
		renderer.setOption( "gl:mode", IECore.StringData( "deferred" ) )
		renderer.worldBegin()
		renderer.setAttribute( "gl:primitive:wireframe", IECore.BoolData( 1 ) )
		renderer.setAttribute( "name", IECore.StringData( "banana" ) )
		if 1 :
			
			#renderer.concatTransform( IECore.M44f.createTranslated( IECore.V3f( 0, 0, -5 ) ) )
			self.__gadget.render( renderer )

		renderer.worldEnd()
		
		self.__scene = renderer.scene()

	def __buttonPress( self, widget, event ) :
	
		print "BUTTON PRESS!", event.x, event.y, event.button
		if not self.__gadget :
			return True
		
		gadgetEvent = ButtonEvent()
		gadgetEvent.buttons = self.__gtkButtonToEventButton( event.button )
		if not gadgetEvent.buttons :
			return True		

		print "FOUND"
		gadget = self.__select( event )
		return True

	def __buttonRelease( self, widget, event ) :
	
		print "BUTTON RELEASE!"
	
	# returns the widget under event.x, event.y
	def __select( self, event ) :
	
		if not self.__scene :
			return None
		
		drawable = self.gtkWidget().get_gl_drawable()
		context = self.gtkWidget().get_gl_context()
		
		if not drawable.gl_begin( context )  :
			return
		
			
		viewportSize = IECore.V2f( self.gtkWidget().allocation.width, self.gtkWidget().allocation.height )
		regionCentre = IECore.V2f( event.x, event.y ) / viewportSize
		regionSize = IECore.V2f( 10 ) / viewportSize
		
		region = IECore.Box2f( regionCentre - regionSize/2, regionCentre + regionSize/2 )
		print region
		
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glColor3f(1.0, 1.0, 1.0);
		glMatrixMode( GL_PROJECTION )
		glLoadIdentity()
		glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0); 
		selection = self.__scene.select( region )
		
		drawable.gl_end()

		if not len( selection ) :
			return None
			
		selection.sort()
		print selection[0].name.value()
		
		return None
	
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
