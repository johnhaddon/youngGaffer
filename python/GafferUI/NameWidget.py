from __future__ import with_statement

import Gaffer
import GafferUI

class NameWidget( GafferUI.TextWidget ) :

	def __init__( self, graphComponent ) :
	
		GafferUI.TextWidget.__init__( self )

		self.setGraphComponent( graphComponent )

		self.gtkWidget().connect( "key-press-event", self.__keyPress )
		self.gtkWidget().connect( "focus-out-event", self.__focusOut )
		
	def setGraphComponent( self, graphComponent ) :
	
		self.__graphComponent = graphComponent	
		self.__nameChangedConnection = self.__graphComponent.nameChangedSignal().connect( lambda x : self.__setText() )
		
		self.__setText()
		
	def getGraphComponent( self ) :
	
		return self.__graphComponent
		
	def __keyPress( self, gtkWidget, event ) :

		# escape abandons everything
		if event.keyval==65307 :
			self.__setText()
			return True

		# return commits any changes made
		if event.keyval==65293 :
			self.__setName()
			return True
			
		return False

	def __focusOut( self, gtkWidget, event ) :
	
		self.__setName()
	
	def __setName( self ) :
	
		with Gaffer.UndoContext( self.__graphComponent.scriptNode() ) :
			self.setText( self.__graphComponent.setName( self.getText() ) )

	def __setText( self ) :
	
		self.setText( self.__graphComponent.getName() )
