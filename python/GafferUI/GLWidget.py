import os

# Setting this environment variable before importing gtkgl prevents "BadColor (invalid Colormap parameter)"
# errors occurring. I'm not sure what causes these errors or what the error really means.
os.environ["GDK_GL_NO_STANDARD_COLORMAP"] = "1"

import gtk
import gtk.gtkgl
from OpenGL.GL import *
import IECoreGL
import IECore

from Widget import Widget

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
		drawingArea = gtk.gtkgl.DrawingArea( glConfig, share_list=self.__glContext() )
		
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
	
	__glContextObject = None # GL context we share among all widgets
	__glContextPixmap = None # pixmap which owns the gl context and keeps it alive for us
	@classmethod	
	def __glContext( cls ) :
		
		if cls.__glContextObject :
			return cls.__glContextObject
		
		config = gtk.gdkgl.Config( mode=gtk.gdkgl.MODE_RGBA | gtk.gdkgl.MODE_DEPTH | gtk.gdkgl.MODE_DOUBLE )	
		cls.__glContextPixmap = gtk.gdk.Pixmap( None, 8, 8, config.get_depth() )
		gtk.gdkgl.ext( cls.__glContextPixmap )
		cls.__glContextPixmap.set_gl_capability( config )
		cls.__glContextObject = gtk.gdkgl.Context( cls.__glContextPixmap.get_gl_drawable() )
		
		return cls.__glContextObject
