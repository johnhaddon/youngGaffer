import gtk

import IECore

import GafferUI

## \todo Worry about colourspace
class ColorSwatch( GafferUI.Widget ) :

	def __init__( self, color=IECore.Color4f( 1 ) ) :
	
		GafferUI.Widget.__init__( self, gtk.DrawingArea() )
	
		self.__color = color
		
		self.gtkWidget().connect( "expose-event", self.__expose )

	def setColor( self, color ) :
	
		if color!=self.__color :
			self.__color = color
			self.gtkWidget().queue_draw()
		
	def getColor( self ) :
	
		return self.__color

	def __expose( self, gtkWidget, event ) :
	
		# create context and clip drawing to the area of the expose
		context = gtkWidget.window.cairo_create()
		context.rectangle( event.area.x, event.area.y, event.area.width, event.area.height )
		context.clip()
		
		allocation = self.gtkWidget().get_allocation()
		
		# draw checkerboard background if necessary
		if self.__color.dimensions()==4 and self.__color.a < 1 :
			checkSize = 6
			nx = 1 + allocation.width / checkSize
			ny = 1 + allocation.height / checkSize
			for x in range( 0, nx ) :
				for y in range( 0, ny ) :
					c = 0.25 + 0.5 * ( ( x + y ) % 2 )
					context.set_source_rgb( c, c, c )
					context.rectangle( x * checkSize, y * checkSize, checkSize, checkSize )
					context.fill()
		
		# draw colour
		if self.__color.dimensions()==4 :
			context.set_source_rgba( self.__color.r, self.__color.g, self.__color.b, self.__color.a )
		else :
			context.set_source_rgb( self.__color.r, self.__color.g, self.__color.b )
		context.rectangle( 0, 0, allocation.width, allocation.height )
		context.fill()
		
		return False
