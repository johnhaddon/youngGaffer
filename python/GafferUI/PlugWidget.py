import gtk

import GafferUI
from PlugValueWidget import PlugValueWidget
import CamelCase

## \todo Make tooltips work
## \todo Useful popup menu on label - connect, disconnect, expressions etc
## Or does that belong on the PlugValueWidget?
class PlugWidget( GafferUI.Widget ) :

	def __init__( self, plugOrPlugValueWidget, label=None, description=None ) :
	
		GafferUI.Widget.__init__( self, gtk.HBox( spacing = 5 ) )

		self.__gtkHBox = self.gtkWidget()
	
		if isinstance( plugOrPlugValueWidget, PlugValueWidget ) :
			self.__valueWidget = plugOrPlugValueWidget
			plug = self.__valueWidget.getPlug()
		else :
			self.__valueWidget = PlugValueWidget.create( plugOrPlugValueWidget )
			plug = plugOrPlugValueWidget

		label = gtk.Label( label or CamelCase.toSpaced( plug.getName() ) )
		label.set_width_chars( 15 )
		label.set_alignment( 1, 0.5 )

		labelEventBox = gtk.EventBox()
		labelEventBox.add( label )
		
		self.__gtkHBox.pack_start( labelEventBox, False )			
		if self.__valueWidget :
			self.__gtkHBox.pack_start( self.__valueWidget.gtkWidget() )

		if description :
			self.__tooltips = gtk.Tooltips()
			self.__tooltips.set_tip( labelEventBox, "hi there" )		
		
		self.__gtkHBox.show_all()
