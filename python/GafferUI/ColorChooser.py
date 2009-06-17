import gtk
import cairo
import IECore

import GafferUI

# A custom slider for drawing the backgrounds.
class ColorSlider( GafferUI.Slider ) :

	def __init__( self, color, component ) :
	
		GafferUI.Slider.__init__( self )
		
		self.color = color
		self.component = component
		
	def setColor( self, color ) :
	
		self.color = color
		self.gtkWidget().queue_draw()
		
	def getColor( self ) :
	
		return self.color	
		
	def _drawBackground( self, context ) :
	
		allocation = self.gtkWidget().get_allocation()
		width = allocation.width
		height = allocation.height

		grad = cairo.LinearGradient( 0, 0, width, 0 )
		
		if self.component=="r" :
			grad.add_color_stop_rgb( 0, 0, self.color[1], self.color[2] )
			grad.add_color_stop_rgb( 1, 1, self.color[1], self.color[2] )
		elif self.component=="g" :
			grad.add_color_stop_rgb( 0, self.color[0], 0, self.color[2] )
			grad.add_color_stop_rgb( 1, self.color[0], 1, self.color[2] )
		elif self.component=="b" :
			grad.add_color_stop_rgb( 0, self.color[0], self.color[1], 0 )
			grad.add_color_stop_rgb( 1, self.color[0], self.color[1], 1 )
		elif self.component in ( "h", "s", "v" ) :
			c1 = IECore.Color3f( self.color[0], self.color[1], self.color[2] ).rgbToHSV()
			c2 = IECore.Color3f( self.color[0], self.color[1], self.color[2] ).rgbToHSV()
			
			a = { "h" : 0, "s" : 1, "v": 2 }[self.component]
			c1[a] = 0
			c2[a] = 1
			
			numStops = 50
			for i in range( 0, numStops ) :
				t = float( i ) / (numStops-1)	
				c = c1 + (c2-c1) * t
				c = c.hsvToRGB()
				grad.add_color_stop_rgb( t, c[0], c[1], c[2] )
			
		context.set_source( grad )
		
		context.rectangle( 0, 0, width, height )
		context.fill()

class ColorChooser( GafferUI.Widget ) :

	def __init__( self, color=IECore.Color3f( 1 ) ) :
	
		self.__column = GafferUI.ListContainer( GafferUI.ListContainer.Orientation.Vertical )
	
		GafferUI.Widget.__init__( self, self.__column.gtkWidget() )

		self.__color = color
		self.__defaultColor = color

		self.__sliders = {}

		self.__sliders["r"] = ColorSlider( color, "r" )
		self.__sliders["g"] = ColorSlider( color, "g" )
		self.__sliders["b"] = ColorSlider( color, "b" )
		self.__column.append( self.__sliders["r"] )
		self.__column.append( self.__sliders["g"] )
		self.__column.append( self.__sliders["b"] )

		self.__sliders["h"] = ColorSlider( color, "h" )
		self.__sliders["s"] = ColorSlider( color, "s" )
		self.__sliders["v"] = ColorSlider( color, "v" )
		self.__column.append( self.__sliders["h"] )
		self.__column.append( self.__sliders["s"] )
		self.__column.append( self.__sliders["v"] )

		self.__sliderConnections = []
		for s in self.__sliders.values() :
			self.__sliderConnections.append( s.positionChangedSignal().connect( self.__sliderChanged ) )

		swatchRow = GafferUI.ListContainer( GafferUI.ListContainer.Orientation.Horizontal )
		self.__initialColorSwatch = GafferUI.ColorSwatch( color )
		self.__initialColorSwatch.gtkWidget().add_events( gtk.gdk.BUTTON_PRESS_MASK )
		self.__initialColorSwatch.gtkWidget().connect( "button-press-event", lambda widget, event : self.setColor( self.getInitialColor() ) )
		swatchRow.append( self.__initialColorSwatch, expand=True )
		self.__colorSwatch = GafferUI.ColorSwatch( color )
		swatchRow.append( self.__colorSwatch, expand=True )
		self.__column.append( swatchRow, expand=True )

		self.__colorChangedSignal = GafferUI.WidgetSignal()
		
		self.__setSlidersFromColor()
	
	## The default color starts as the value passed when creating the dialogue.
	# It is represented with a swatch which when clicked will revert the current
	# selection back to the original.
	def setInitialColor( self, color ) :
	
		self.__initialColorSwatch.setColor( color )
		
	def getInitialColor( self ) :
	
		return self.__initialColorSwatch.getColor()
		
	def setColor( self, color ) :
	
		if color!=self.__color :
		
			self.__color = color
			self.__setSlidersFromColor()
			self.__colorSwatch.setColor( color )
			self.__colorChangedSignal( self )
			
	def getColor( self ) :
	
		return self.__color

	def colorChangedSignal( self ) :
	
		return self.__colorChangedSignal

	def __sliderChanged( self, slider ) :
			
		if slider.component in ( "r", "g", "b" ) :
			color = IECore.Color3f( self.__sliders["r"].getPosition(), self.__sliders["g"].getPosition(), self.__sliders["b"].getPosition() )
		else :
			color = IECore.Color3f( self.__sliders["h"].getPosition(), self.__sliders["s"].getPosition(), self.__sliders["v"].getPosition() )
			color = color.hsvToRGB()
			
		for slider in self.__sliders.values() :
			slider.setColor( color )		

		self.setColor( color )
		
	def __setSlidersFromColor( self ) :
	
		try :

			for co in self.__sliderConnections :
				co.block()
	
			c = self.getColor()
			self.__sliders["r"].setPosition( c[0] )
			self.__sliders["g"].setPosition( c[1] )
			self.__sliders["b"].setPosition( c[2] )

			c = c.rgbToHSV()
			self.__sliders["h"].setPosition( c[0] )
			self.__sliders["s"].setPosition( c[1] )
			self.__sliders["v"].setPosition( c[2] )

		finally :
		
			for co in self.__sliderConnections :
				co.unblock()
