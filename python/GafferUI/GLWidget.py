from Widget import Widget
import gtk
import gtk.gtkgl
from OpenGL.GL import *
import IECoreGL
import IECore

## The GLWidget is a base class for all widgets which wish to draw using OpenGL.
# Derived classes override the draw() method to achieve this.
class GLWidget( Widget ) :

	## This enum defines the optional elements of the GL buffer used
	# for display.
	BufferOptions = IECore.Enum.create(
		"Alpha",
		"Depth",
		"Double"
	)	
	
	__glContext = None ## Context we share among all widgets
	
	def __init__( self, bufferOptions = set() ) :
	
		if self.BufferOptions.Alpha in bufferOptions :
			displayMode = gtk.gdkgl.MODE_RGBA
		else :
			displayMode = gtk.gdkgl.MODE_RGB
		
		if self.BufferOptions.Depth in bufferOptions :
			displayMode |= gtk.gdkgl.MODE_DEPTH 
		
		if self.BufferOptions.Double in bufferOptions :
			displayMode |= gtk.gdkgl.MODE_DOUBLE 
			
		glConfig = gtk.gdkgl.Config( mode=displayMode )
		drawingArea = gtk.gtkgl.DrawingArea( glConfig, share_list=self.__glContext )
		
		Widget.__init__( self, drawingArea )
				
		drawingArea.connect( "configure_event", self.__configure )
		drawingArea.connect( "expose_event", self.__expose )
		drawingArea.connect_after( "realize", self.__realize )
		drawingArea.show()

	## Derived classes must override this to draw their contents using
	# OpenGL calls.
	def draw( self ) :
	
		raise NotImplementedError
		
	def __realize( self, widget ) :
	
		# we need to call the init method after a GL context has been
		# created, and this seems like as good a place as any. it does
		# mean we call init() once for every widget created, but it's
		# safe to call init() multiple times anyway.
		IECoreGL.init( True )
		## \todo this context dies or something when a glwidget is reparented
		# there is a suggestion as to how to fix this here :
		#
		# http://markmail.org/message/zkbiilfe45rpbiv2#query:gtkglext%20reparenting+page:1+mid:zkbiilfe45rpbiv2+state:results
		#
		# but I couldn't get it to work yet
		if not GLWidget.__glContext :
			# we're the first gl widget, save the context for subsequent ones
			GLWidget.__glContext = widget.get_gl_context()
			
	def __configure( self, widget, event ) :
	
		drawable = widget.get_gl_drawable()
		context = widget.get_gl_context()
		
		if not drawable.gl_begin( context ) :
			return
		
		glViewport( 0, 0, widget.allocation.width, widget.allocation.height )
			
		drawable.gl_end()

	def __expose( self, widget, event ) :
	
		drawable = widget.get_gl_drawable()
		context = widget.get_gl_context()
		
		if not drawable.gl_begin( context )  :
			return
			
		self.draw()
		
		if drawable.is_double_buffered() :
			drawable.swap_buffers()
		else :
			glFlush()
			
		drawable.gl_end()
		
