import pango
import gtk

import Gaffer
from PlugValueWidget import PlugValueWidget

## User docs :
#
# Control left click increments the digit below the mouse
# Control shift left click decrements the digit below the mouse
# Control up increments the digit to the right of the cursor
# Control down decrements the digit to the right of the cursor
# Return commits any changes
# Tab commits any changes and moves focus to the next widget
# Escape abandons any uncommitted changes
# Deleting all text and committing reverts to the default value.
#
## \todo Maths expressions to modify the existing value
## \todo Enter names of other plugs to create a connection
## \todo Color change for connected plugs and output plugs
## \todo Reject drag and drop of anything that's not a number
class NumericPlugValueWidget( PlugValueWidget ) :

	def __init__( self, plug ) :
	
		PlugValueWidget.__init__( self, gtk.Entry(), plug )

		self.gtkEntry = self.gtkWidget()
		self.gtkEntry.connect( "key-press-event", self.__keyPress )
		self.gtkEntry.connect( "button-press-event", self.__buttonPress )
		self.gtkEntry.connect( "focus-out-event", self.__focusOut )
		
		self._setColors( self.gtkEntry, self.State.Normal, self._textEntryFGColor, self._textEntryBGColor )
		
		self.gtkEntry.show()
		
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
		
		self.gtkEntry.set_editable( self._editable() )

	def __buttonPress( self, widget, event ) :
	
		editable = self.gtkEntry.get_editable()

		if event.button==1 and event.state & gtk.gdk.CONTROL_MASK :
		
			# ctrl left click
			if editable :

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

		editable = self.gtkEntry.get_editable()
	
		# control up/down nudge values
		if event.keyval==65362 and event.state & gtk.gdk.CONTROL_MASK :
			if editable :
				self.__incrementIndex( self.gtkEntry.get_position(), 1 )
			return True
		
		if event.keyval==65364 and event.state & gtk.gdk.CONTROL_MASK :
			if editable :
				self.__incrementIndex( self.gtkEntry.get_position(), -1 )
			return True
			
		# escape abandons everything
		if event.keyval==65307 :
			if editable :
				self.updateFromPlug()
			return True
			
		# return commits any changes made
		if event.keyval==65293 :
			if editable :
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
			if self.gtkEntry.get_position()!=0 :
				return True
			text = self.gtkEntry.get_text()
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
		
		# pass through ctrl shortcuts we want to be processed normally
		if event.state & gtk.gdk.CONTROL_MASK :
			keysWeLike = [
				97, # A
				99, # C
				118, # V
				120, # X
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
		if text=="" :
			# revert to default
			self.getPlug().setValue( self.getPlug().defaultValue() )
		
		try :	
			self.getPlug().setValue( self.__numericType()( text ) )
		except :
			self.updateFromPlug()	
	
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
		self.gtkEntry.set_position( newIndex )

PlugValueWidget.registerType( Gaffer.FloatPlug.staticTypeId(), NumericPlugValueWidget )
PlugValueWidget.registerType( Gaffer.IntPlug.staticTypeId(), NumericPlugValueWidget )

