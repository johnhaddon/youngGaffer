import gtk

import Gaffer
import GafferUI

class PathChooserDialogue( GafferUI.Dialogue ) :

	def __init__( self, path, title="Select path", cancelLabel="Cancel", confirmLabel="OK" ) :
	
		GafferUI.Dialogue.__init__( self, title )
		
		self.__path = path
		
		self.__pathChooserWidget = GafferUI.PathChooserWidget( path )
		self._setWidget( self.__pathChooserWidget )
		self.__pathChooserSelectedConnection = self.__pathChooserWidget.pathSelectedSignal().connect( self.__pathChooserSelected )

		self.__cancelButton = self._addButton( cancelLabel )
		self.__cancelButtonConnection = self.__cancelButton.clickedSignal().connect( self.__buttonClicked )
		self.__confirmButton = self._addButton( confirmLabel )
		self.__confirmButtonConnection = self.__confirmButton.clickedSignal().connect( self.__buttonClicked )
		
		self.__pathSelectedSignal = Gaffer.ObjectSignal()
	
	## A signal called when a path has been selected. Slots for this signal
	# should accept a single argument which will be the PathChooserDialogue instance.	
	def pathSelectedSignal( self ) :
	
		return self.__pathSelectedSignal
	
	## Causes the dialogue to enter a modal state, returning the path once it has been
	# selected by the user. Returns None if the dialogue is cancelled.
	def waitForPath( self ) :
	
		button = self.waitForButton()
		if button is self.__confirmButton :
			return self.__path.copy()
			
		return None
		
	def __buttonClicked( self, button ) :
	
		if button is self.__confirmButton :
			self.pathSelectedSignal()( self )
	
	def __pathChooserSelected( self, pathChooser ) :
	
		assert( pathChooser is self.__pathChooserWidget )
		self.__confirmButton.clickedSignal()( self.__confirmButton )	
		
	
