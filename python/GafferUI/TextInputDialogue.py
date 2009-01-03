import Gaffer
import GafferUI

class TextInputDialogue( GafferUI.Dialogue ) :

	def __init__( self, initialText="", title="Enter text", cancelLabel="Cancel", confirmLabel="OK") :
	
		GafferUI.Dialogue.__init__( self, title )
		
		self.__textWidget = GafferUI.TextWidget( initialText )
		self._setWidget( self.__textWidget )
		self.__textActivatedConnection = self.__textWidget.activatedSignal().connect( self.__textActivated )

		self.__cancelButton = self._addButton( cancelLabel )
		self.__confirmButton = self._addButton( confirmLabel )
		
		self.setResizeable( False )
		
	def waitForText( self ) :
	
		button = self.waitForButton()
		if button is self.__confirmButton :
			return self.__textWidget.getText()
		
		return None

	def __textActivated( self, textWidget ) :
	
		assert( textWidget is self.__textWidget )
		
		self.__confirmButton.clickedSignal()( self.__confirmButton )
