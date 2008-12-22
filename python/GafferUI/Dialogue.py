import GafferUI

class Dialogue( GafferUI.Window ) :

	def __init__( self, title ) :
	
		GafferUI.Window.__init__( self, title )
		
		self.__column = GafferUI.ListContainer( GafferUI.ListContainer.Orientation.Vertical )
		
		self.__widget = GafferUI.Button() ## \todo Need some kind of empty widget
		self.__column.append( self.__widget, True )
		
		self.__buttonRow = GafferUI.ListContainer( GafferUI.ListContainer.Orientation.Horizontal, homogeneous=True )
		self.__column.append( self.__buttonRow )
		
		self.setChild( self.__column )
	
	## \bug Hitting Ctrl-C during this call doesn't close the app.
	def waitForButton( self ) :
	
		assert( len( self.__buttonRow ) )
	
		self.gtkWidget().set_modal( True )
		
		self.__buttonConnections = []
		self.__closedConnection = self.closedSignal().connect( self.__closed )
		for button in self.__buttonRow :
			self.__buttonConnections.append( button.clickedSignal().connect( self.__buttonClicked ) )
		
		self.__resultOfWait = None
		GafferUI.EventLoop.start()
		self.__buttonConnections = []
		self.__closedConnection = None

		self.gtkWidget().set_modal( False )
		
		return self.__resultOfWait
		
	def _setWidget( self, widget ) :
	
		self.__column[0] = widget
		
	def _getWidget( self, widget ) :
		
		return self.__column[0]
		
	def _addButton( self, label ) :
		
		button = GafferUI.Button( label=label )
		self.__buttonRow.append( button )
		return button
		
	def __buttonClicked( self, button ) :
	
		# check we're in a call to _waitForButton
		assert( len( self.__buttonConnections ) )
		self.__resultOfWait = button
		GafferUI.EventLoop.stop()
		
	def __closed( self, widget ) :
	
		# check we're in a call to _waitForButton
		assert( len( self.__buttonConnections ) )

		self.__resultOfWait = None
		GafferUI.EventLoop.stop()
