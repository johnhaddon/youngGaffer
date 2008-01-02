from Widget import Widget
from PlugValueWidget import PlugValueWidget
import gtk

## \todo Make tooltips work
## \todo Useful popup menu on label - connect, disconnect, expressions etc
class PlugWidget( Widget ) :

	def __init__( self, plugOrPlugValueWidget, label=None, description=None ) :
	
		Widget.__init__( self )
	
		plugValueWidget = plugOrPlugValueWidget
		if not isinstance( plugValueWidget, PlugValueWidget ) :
			plugValueWidget = PlugValueWidget.create( plugValueWidget )
		
		self.__gtkHBox = gtk.HBox( spacing = 5 )
		self.setGTKWidget( self.__gtkHBox )
		
		label = gtk.Label( label or plugValueWidget.getPlug().getName() )
		label.set_width_chars( 15 )
		label.set_alignment( 1, 0.5 )

		labelEventBox = gtk.EventBox()
		labelEventBox.add( label )
		
		self.__gtkHBox.pack_start( labelEventBox, False )			
		self.__gtkHBox.pack_start( plugValueWidget.getGTKWidget() )

		if description :
			self.__tooltips = gtk.Tooltips()
			self.__tooltips.set_tip( labelEventBox, "hi there" )
