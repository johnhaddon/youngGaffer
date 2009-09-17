import gtk

import IECore

import Gaffer
import GafferUI

class ColorPlugValueWidget( GafferUI.PlugValueWidget ) :

	def __init__( self, plug ) :
	
		self.__swatch = GafferUI.ColorSwatch()
		
		GafferUI.PlugValueWidget.__init__( self, self.__swatch.gtkWidget(), plug )
		
		self.__swatch.gtkWidget().add_events( gtk.gdk.BUTTON_PRESS_MASK )
		self.__swatch.gtkWidget().connect( "button-press-event", self.__buttonPress )
		
		self.__colorChooserDialogue = None
		
	def updateFromPlug( self ) :
	
		plug = self.getPlug()
		c = plug.getValue()
		self.__swatch.setColor( c )
		
	def __buttonPress( self, widget, event ) :
		
		if not self.__colorChooserDialogue :	
			self.__colorChooserDialogue = GafferUI.ColorChooserDialogue()
		
		self.ancestor( GafferUI.Window ).addChildWindow( self.__colorChooserDialogue )
		self.__colorChooserDialogue.setTitle( self.getPlug().relativeName( self.getPlug().ancestor( Gaffer.ScriptNode.staticTypeId() ) ) )
		self.__colorChooserDialogue.colorChooser().setColor( self.getPlug().getValue() )
		self.__colorChooserDialogue.colorChooser().setInitialColor( self.getPlug().getValue() )
		
		self.__colorChangedConnection = self.__colorChooserDialogue.colorChooser().colorChangedSignal().connect( self.__colorChanged )
		self.__confirmClickedConnection = self.__colorChooserDialogue.confirmButton.clickedSignal().connect( self.__buttonClicked )
		self.__cancelClickedConnection = self.__colorChooserDialogue.cancelButton.clickedSignal().connect( self.__buttonClicked )
		self.__dialogueClosedConnection = self.__colorChooserDialogue.closedSignal().connect( self.__dialogueClosed )
		
		self.__colorChooserDialogue.show()
		
	def __colorChanged( self, colorDialogue ) :
	
		color = colorDialogue.getColor()
		self.getPlug().setValue( color )
		
	def __buttonClicked( self, button ) :
	
		if button is self.__colorChooserDialogue.cancelButton :
			self.getPlug().setValue( self.__colorChooserDialogue.colorChooser().getInitialColor() )
			
		self.__colorChooserDialogue.close()
		self.__colorChooserDialogue = None
		
	def __dialogueClosed( self, dialogue ) :
	
		self.__colorChooserDialogue = None
				
GafferUI.PlugValueWidget.registerType( Gaffer.Color3fPlug.staticTypeId(), ColorPlugValueWidget )
GafferUI.PlugValueWidget.registerType( Gaffer.Color4fPlug.staticTypeId(), ColorPlugValueWidget )
