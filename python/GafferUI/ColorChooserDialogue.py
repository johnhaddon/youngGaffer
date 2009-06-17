import IECore

import Gaffer
import GafferUI

class ColorChooserDialogue( GafferUI.Dialogue ) :

	def __init__( self, title="Select color", color=IECore.Color3f( 1 ), cancelLabel="Cancel", confirmLabel="OK" ) :
	
		GafferUI.Dialogue.__init__( self, title )
		
		self.__colorChooserWidget = GafferUI.ColorChooser()
		self._setWidget( self.__colorChooserWidget )

		self.cancelButton = self._addButton( cancelLabel )
		self.confirmButton = self._addButton( confirmLabel )
			
	## Returns the embedded ColorChooser for manipulation.
	def colorChooser( self ) :
	
		return self._getWidget()
		
	## Causes the dialogue to enter a modal state, returning the color once it has been
	# selected by the user. Returns None if the dialogue is cancelled.
	def waitForColor( self ) :
	
		button = self.waitForButton()
		if button is self.__confirmButton :
			return self.colorChooser().getColor()
			
		return None
