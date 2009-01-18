import re

import IECore

import Gaffer
import GafferUI

class ScriptWindow( GafferUI.Window ) :

	def __init__( self, script ) :
	
		GafferUI.Window.__init__( self )

		self.__script = script
		
		self.__listContainer = GafferUI.ListContainer( GafferUI.ListContainer.Orientation.Vertical )
		self.__listContainer.show()
		
		m = GafferUI.MenuBar( self.menuDefinition() )
		
		self.__listContainer.append( m )
		
		if "Default" in GafferUI.Layouts.names() :
			self.setLayout( GafferUI.Layouts.create( "Default" ) )
		else :
			self.setLayout( GafferUI.CompoundEditor() )
				
		self.setChild( self.__listContainer )
		
		scriptParent = script.parent()
		if scriptParent :
			self.__scriptRemovedConnection = scriptParent.childRemovedSignal().connect( self.__scriptRemoved )

		self.__closedConnection = self.closedSignal().connect( self.__closed )

		self.__scriptPlugSetConnection = script.plugSetSignal().connect( self.__scriptPlugChanged )
		self.__scriptPlugDirtiedConnection = script.plugDirtiedSignal().connect( self.__scriptPlugChanged )
	
		self.__updateTitle()

		self._setDefaultColors( self.gtkWidget() )

	## \todo Implement setScript() - and decide on naming so it matches the Editor method (getScriptNode)
	def getScript( self ) :
	
		return self.__script

	def setLayout( self, compoundEditor ) :
	
		if len( self.__listContainer ) > 1 :
			del self.__listContainer[1]
	
		compoundEditor.setScriptNode( self.__script )
		self.__listContainer.append( compoundEditor, expand=True )
		
	def getLayout( self ) :
	
		return self.__listContainer[1] 

	def __closed( self, widget ) :
		
		scriptParent = self.__script.parent()
		if scriptParent :
			scriptParent.removeChild( self.__script )
			
	def __scriptRemoved( self, scriptContainer, script ) :
	
		if script.isSame( self.__script ) :
			ScriptWindow.__instances.remove( self )

	def __scriptPlugChanged( self, plug ) :
	
		if plug.isSame( self.__script["fileName"] ) :
			self.__updateTitle()
	
	def __updateTitle( self ) :
	
		f = self.__script["fileName"].getValue()
		if not f :
			f = "untitled"
			d = ""
		else :
			d, n, f = f.rpartition( "/" )
			d = " - " + d
			
		self.setTitle( "Gaffer : %s %s" % ( f, d ) )

	## Returns an IECore.MenuDefinition which is used to define the menu bars for all ScriptWindows.
	# This can be edited at any time to modify subsequently created ScriptWindows - typically editing
	# would be done as part of gaffer startup.
	@staticmethod
	def menuDefinition() :
	
		return ScriptWindow.__menuDefinition
	
	__menuDefinition = IECore.MenuDefinition()	

	## This function provides the top level functionality for instantiating
	# the UI. Once called, new ScriptWindows will be instantiated for each
	# script added to the application, and the EventLoop will be terminated
	# when the last script is removed.
	@staticmethod
	def connect( applicationRoot ) :
	
		ScriptWindow.__scriptAddedConnection = applicationRoot["scripts"].childAddedSignal().connect( ScriptWindow.__scriptAdded )
		ScriptWindow.__scriptRemovedConnection = applicationRoot["scripts"].childRemovedSignal().connect( ScriptWindow.__staticScriptRemoved )

	__instances = []
	@staticmethod
	def __scriptAdded( scriptContainer, script ) :
	
		ScriptWindow.__instances.append( ScriptWindow( script ) )
		
	@staticmethod
	def __staticScriptRemoved( scriptContainer, script ) :
	
		if not len( scriptContainer.children() ) :
			
			GafferUI.EventLoop.stop()
