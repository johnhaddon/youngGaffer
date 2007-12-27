from PlugValueWidget import PlugValueWidget
import Gaffer
import pango
import gtk

## User docs :
#
# Control left click increments the digit below the mouse
# Control shift left click decrements the digit below the mouse
# Control up increments the digit to the right of the cursor
# Control down decrements the digit to the right of the cursor
# Return commits any changes
# Tab commits any changes and moves focus to the next widget
# Escape abandons any uncommitted changes
#
## \todo Maths expressions to modify the existing value
## \todo Enter names of other plugs to create a connection
## \todo Color change for connected plugs
## \todo Reject drag and drop of anything that's not a number
## \todo Fix bug caused by trying to set an empty string as the value
## \todo Add support for Ctrl-A, and any other hotkeys we're masking
class NumericPlugValueWidget( PlugValueWidget ) :

	def __init__( self, plug ) :
	
		PlugValueWidget.__init__( self, plug )

		self.gtkWidget = gtk.Entry()
		self.gtkEntry = self.gtkWidget
		self.gtkEntry.connect( "key-press-event", self.__keyPress )
		self.gtkEntry.connect( "button-press-event", self.__buttonPress )
		self.gtkEntry.connect( "focus-out-event", self.__focusOut )
		## \todo Consider the style of these widgets
		# self.gtkEntry.set_has_frame( False )
		self.updateFromPlug()
		
	def updateFromPlug( self ) :

		if not hasattr( self, "gtkEntry" ) :
			# we're still constructing
			return

		value = self.getPlug().getValue()
		if self.__floatPlug() :
			text = "%.4f" % value
		else :
			text = "%d" % value

		self.gtkEntry.set_text( text )

	def __buttonPress( self, widget, event ) :
	
		if event.button==1 and event.state & gtk.gdk.CONTROL_MASK :
		
			# ctrl left click
			
			layout = self.gtkEntry.get_layout()
			offset = self.gtkEntry.get_layout_offsets()
			cursor = layout.xy_to_index(
				int( (event.x - offset[0]) * pango.SCALE ),
				int( (event.y - offset[1]) * pango.SCALE )
			)

			step = 1
			if event.state & gtk.gdk.SHIFT_MASK :
				step = -1
			
			self.__incrementIndex( cursor[0], step )
			
			return True
			
		return False
	
	def __keyPress( self, widget, event ) :
	
		# process special actions
		###########################################################
	
		# control up/down nudge values
		if event.keyval==65362 and event.state & gtk.gdk.CONTROL_MASK :
			self.__incrementIndex( self.gtkEntry.get_position(), 1 )
			return True
		
		if event.keyval==65364 and event.state & gtk.gdk.CONTROL_MASK :
			self.__incrementIndex( self.gtkEntry.get_position(), -1 )
			return True
			
		# escape abandons everything
		if event.keyval==65307 :
			self.updateFromPlug()
			return True
			
		# return commits any changes made
		if event.keyval==65293 :
			self.__setPlugValue()
			return True
		
		# control normal character entry
		#############################################################
		
		# suppress decimal point for ints, and make
		# sure there's never more than one for floats
		if event.string=="." :
			if not self.__floatPlug() :
				return True
			if "." in self.gtkEntry.get_text() :
				return True
			else :
				return False
			
		# accept 0-9 as normal input
		if event.keyval >= 48 and event.keyval <= 57 :
			return False
			
		# accept - only as the first character, and only if
		# it's the only one
		if event.keyval==45 :
			if self.gtkWidget.get_position()!=0 :
				return True
			text = self.gtkWidget.get_text()
			if '-' in text :
				return True
			return False
				 						
		# pass through things we want to be processed normally
		keysWeLike = [
			65361, # left
			65363, # right
			65362, # up
			65364, # down
			65289, # tab
			65288, # backspace
		]
		if event.keyval in keysWeLike :
			return False
		
		# suppress processing of all other keypresses so we don't get unwanted text
		return True
		
	def __focusOut( self, widget, event ) :
	
		self.__setPlugValue()	
		
	def __floatPlug( self ) :
	
		return self.getPlug().isInstanceOf( Gaffer.FloatPlug.staticTypeId() )
	
	## \todo Should this be available from the NumericPlug somehow?	
	def __numericType( self ) :
	
		if self.__floatPlug() :
			return float
		else :
			return int
	
	def __setPlugValue( self ) :
	
		text = self.gtkEntry.get_text()
		self.getPlug().setValue( self.__numericType()( text ) )		
	
	def __decimalPointIndex( self, s ) :
	
		i = s.find( "." )
		if i != -1 :
			return i
		else :
			return len( s ) - 1
		
	def __incrementIndex( self, index, increment ) :
	
		text = self.gtkEntry.get_text()
		if '.' in text :
			decimalIndex = text.find( "." )
			if decimalIndex >= index :
				index += 1
		else :
			decimalIndex = len( text ) - 1
			
		powIndex = decimalIndex - index
		
		value = self.__numericType()( text )			
		value += increment * self.__numericType()( pow( 10, powIndex ) )
			
		# this triggers a callback and changes the entry text
		self.getPlug().setValue( value )
			
		# adjust the cursor position to be in the same column as before
		newText = self.gtkEntry.get_text()
		if '.' in newText :
			newDecimalIndex = newText.find( "." )
			newIndex = newDecimalIndex - powIndex
			if powIndex >= 0 :
				newIndex -= 1
		else :
			newIndex = len( newText ) - 1 - powIndex
		if newIndex < 0 :
			newIndex = 0
		self.gtkWidget.set_position( newIndex )
