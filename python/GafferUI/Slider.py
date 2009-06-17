import math

import gtk

import GafferUI

class Slider( GafferUI.Widget ) :

	def __init__( self, position = 0.5 ) :
	
		GafferUI.Widget.__init__( self, gtk.DrawingArea() )
		
		self.__position = position

		self.gtkWidget().add_events( gtk.gdk.BUTTON_PRESS_MASK | gtk.gdk.BUTTON1_MOTION_MASK | gtk.gdk.POINTER_MOTION_HINT_MASK )

		self.gtkWidget().connect( "expose-event", self.__expose )		
		self.gtkWidget().connect( "button-press-event", self.__buttonPress )
		self.gtkWidget().connect( "motion_notify_event", self.__motionNotify )
		
		self.gtkWidget().set_size_request( 20, 20 )
		
	def setPosition( self, p ) :
		
		if p!=self.__position :
		
			self.__position = p
			self.gtkWidget().queue_draw()
			
			try :
				signal = self.__positionChangedSignal
			except :
				return
			
			signal( self )
			
	def getPosition( self ) :
	
		return self.__position	
	
	def positionChangedSignal( self ) :
	
		try :
			return self.__positionChangedSignal
		except :
			self.__positionChangedSignal = GafferUI.WidgetSignal()
			
		return self.__positionChangedSignal
	
	def _drawBackground( self, context ) :
	
		allocation = self.gtkWidget().get_allocation()
		width = allocation.width
		height = allocation.height

		context.set_line_width( 1 )
		context.set_source_rgb( 0.05, 0.05, 0.05 )
		context.move_to( 0, height / 2.0 )
		context.line_to( width, height / 2.0 )
		context.stroke()
		
	def _drawPosition( self, context ) :
	
		allocation = self.gtkWidget().get_allocation()
		width = allocation.width
		height = allocation.height

		context.set_source_rgb( 0.5, 0.5, 0.5 )
		context.arc( self.__position * width, height / 2.0, height / 4.0, 0, math.pi * 2.0 )
		context.fill_preserve() 
		context.set_source_rgb( 0.05, 0.05, 0.05 )
		context.stroke()
		
	def __expose( self, gtkWidget, event ) :
	
		context = gtkWidget.window.cairo_create()
		context.rectangle( event.area.x, event.area.y, event.area.width, event.area.height )
		context.clip()
		
		self._drawBackground( context )
		self._drawPosition( context )
			
	def __buttonPress( self, gtkWidget, event ) :
	
		if event.button==1 :
			allocation = gtkWidget.get_allocation()
			self.setPosition( float( event.x ) / allocation.width )

	def __motionNotify( self, gtkWidget, event ) :
	
		x, y, state = event.window.get_pointer()
		allocation = gtkWidget.get_allocation()
		self.setPosition( float( x ) / allocation.width )

