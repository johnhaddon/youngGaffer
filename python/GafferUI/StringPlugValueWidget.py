from PlugValueWidget import PlugValueWidget
import Gaffer
import gtk

## User docs :
#
# Return commits any changes onto the plug.
# Escape abandons any uncommitted changes.
class StringPlugValueWidget( PlugValueWidget ) :

	def __init__( self, plug ) :
	
		PlugValueWidget.__init__( self, plug )

		self.gtkWidget = gtk.Entry()
		self.gtkEntry = self.gtkWidget
		self.gtkEntry.connect( "key-press-event", self.__keyPress )
		self.gtkEntry.connect( "focus-out-event", self.__focusOut )

		self.updateFromPlug()

	def updateFromPlug( self ) :

		if not hasattr( self, "gtkEntry" ) :
			# we're still constructing
			return

		self.gtkEntry.set_text( self.getPlug().getValue() )

	def __keyPress( self, widget, event ) :
	
		# escape abandons everything
		if event.keyval==65307 :
			self.updateFromPlug()
			return True

		# return commits any changes made
		if event.keyval==65293 :
			self.__setPlugValue()
			return True
			
		return False
		
	def __focusOut( self, widget, event ) :
	
		self.__setPlugValue()
		
	def __setPlugValue( self ) :
	
		text = self.gtkEntry.get_text()
		self.getPlug().setValue( text )

PlugValueWidget.registerType( Gaffer.StringPlug.staticTypeId(), StringPlugValueWidget )
