from Widget import Widget
from PlugValueWidget import PlugValueWidget
import CamelCase

import gtk

## \todo Make tooltips work
## \todo Useful popup menu on label - connect, disconnect, expressions etc
class PlugWidget( Widget ) :

	def __init__( self, plugOrPlugValueWidget, label=None, description=None ) :
	
		Widget.__init__( self, gtk.HBox( spacing = 5 ) )

		self.__gtkHBox = self.gtkWidget()
	
		if isinstance( plugOrPlugValueWidget, PlugValueWidget ) :
			self.__valueWidget = plugOrPlugValueWidget
		else :
			self.__valueWidget = PlugValueWidget.create( plugOrPlugValueWidget )

		label = gtk.Label( label or CamelCase.toSpaced( self.__valueWidget.getPlug().getName() ) )
		label.set_width_chars( 15 )
		label.set_alignment( 1, 0.5 )

		labelEventBox = gtk.EventBox()
		labelEventBox.add( label )
		
		self.__gtkHBox.pack_start( labelEventBox, False )			
		self.__gtkHBox.pack_start( self.__valueWidget.gtkWidget() )

		if description :
			self.__tooltips = gtk.Tooltips()
			self.__tooltips.set_tip( labelEventBox, "hi there" )
