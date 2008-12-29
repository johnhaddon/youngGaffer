import re

import IECore

import GafferUI
import Gaffer

class ScriptWindow( GafferUI.Window ) :

	def __init__( self, script ) :
	
		GafferUI.Window.__init__( self )

		self.__script = script
		
		l = GafferUI.ListContainer( GafferUI.ListContainer.Orientation.Vertical )
		l.show()
		
		m = GafferUI.MenuBar( self.menuDefinition() )
		
		p = GafferUI.CompoundEditor( self.__script )
		
		l.append( m )
		l.append( p, expand=True )
		
		self.setChild( l )
		
		scriptParent = script.parent()
		if scriptParent :
			self.__scriptRemovedConnection = scriptParent.childRemovedSignal().connect( self.__scriptRemoved )

		self.__closedConnection = self.closedSignal().connect( self.__closed )

		self.__scriptPlugSetConnection = script.plugSetSignal().connect( self.__scriptPlugChanged )
		self.__scriptPlugDirtiedConnection = script.plugDirtiedSignal().connect( self.__scriptPlugChanged )
	
		self.__updateTitle()

	## \todo Implement setScript()
	def getScript( self ) :
	
		return self.__script

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
