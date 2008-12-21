import GafferUI
import gtk

class Button( GafferUI.Widget ) :

	def __init__( self, label="Button" ) :
	
		GafferUI.Widget.__init__( self, gtk.Button( label=label ) )
		self.gtkWidget().connect( "clicked", self.__clicked )
		
		self.__clickedSignal = GafferUI.WidgetSignal()
				
	def clickedSignal( self ) :
	
		return self.__clickedSignal
		
	def __clicked( self, gtkButton ) :
	
		self.clickedSignal()( self )
