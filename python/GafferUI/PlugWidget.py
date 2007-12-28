from PlugValueWidget import PlugValueWidget
import gtk

## \todo Make tooltips work
## \todo Useful popup menu on label - connect, disconnect, expressions etc
class PlugWidget() :

	def __init__( self, plugOrPlugValueWidget, label=None, description=None ) :
	
		plugValueWidget = plugOrPlugValueWidget
		if not isinstance( plugValueWidget, PlugValueWidget ) :
			plugValueWidget = PlugValueWidget.create( plugValueWidget )
		
		self.gtkWidget = gtk.HBox( spacing = 5 )
		self.gtkHBox = self.gtkWidget
		
		label = gtk.Label( label or plugValueWidget.getPlug().getName() )
		label.set_width_chars( 15 )
		label.set_alignment( 1, 0.5 )

		labelEventBox = gtk.EventBox()
		labelEventBox.add( label )
		
		self.gtkHBox.pack_start( labelEventBox, False )			
		self.gtkHBox.pack_start( plugValueWidget.gtkWidget )

		if description :
			self.__tooltips = gtk.Tooltips()
			self.__tooltips.set_tip( labelEventBox, "hi there" )
