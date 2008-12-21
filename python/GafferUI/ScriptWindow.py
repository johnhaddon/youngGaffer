import re

import gtk
import IECore

import GafferUI
import Gaffer

class ScriptWindow( GafferUI.Window ) :

	def __menuFileNew( menu ) :

		scriptWindow = menu.ancestor( GafferUI.ScriptWindow )
		application = scriptWindow.getScript().ancestor( Gaffer.ApplicationRoot.staticTypeId() )
		
		newScript = Gaffer.ScriptNode( "script" )
		application["scripts"].addChild( newScript )
		
	def __menuFileOpen( menu ) :
	
		path = Gaffer.FileSystemPath( "/" )
		path.addFilter( Gaffer.FileNamePathFilter( [ "*.gfr" ] ) )
		path.addFilter( Gaffer.FileNamePathFilter( [ re.compile( "^[^.].*" ) ], leafOnly=False ) )
		dialogue = GafferUI.PathChooserDialogue( path, title="Open script", confirmLabel="Open" )
		path = dialogue.waitForPath()
		dialogue.close()		
		
		if not path :
			return
		
		newScript = Gaffer.ScriptNode()
		scriptWindow = menu.ancestor( GafferUI.ScriptWindow )
		application = scriptWindow.getScript().ancestor( Gaffer.ApplicationRoot.staticTypeId() )

		newScript["fileName"].setValue( str( path ) )
		newScript.load()

		application["scripts"].addChild( newScript )
	
	def __menuFileSave( menu ) :
	
		scriptWindow = menu.ancestor( GafferUI.ScriptWindow )
		script = scriptWindow.getScript()
		if script["fileName"].getValue() :
			script.save()
		else :
			ScriptWindow.__menuFileSaveAs( menu )
	
	def __menuFileSaveAs( menu ) :
	
		scriptWindow = menu.ancestor( GafferUI.ScriptWindow )
		script = scriptWindow.getScript()
		
		path = Gaffer.FileSystemPath( script["fileName"].getValue() )
		path.addFilter( Gaffer.FileNamePathFilter( [ "*.gfr" ] ) )
		path.addFilter( Gaffer.FileNamePathFilter( [ re.compile( "^[^.].*" ) ], leafOnly=False ) )
		
		dialogue = GafferUI.PathChooserDialogue( path, title="Save script", confirmLabel="Save" )
		path = dialogue.waitForPath()
		dialogue.close()		
		
		if not path :
			return
			
		script["fileName"].setValue( str( path ) )
		script.save()
	
	def __menuFileRevertToSaved( menu ) :
	
		scriptWindow = menu.ancestor( GafferUI.ScriptWindow )
		script = scriptWindow.getScript()
		
		if script["fileName"].getValue() :
			script.load()
		else :
			## \todo Warn
			pass
		
	## \todo We need a mechanism for different applications to add
	# different custom things to this menu and without stomping over each
	# other too. perhaps all the entries should be added during the config
	# loading phase?
	menuDefinition = IECore.MenuDefinition(
	
		[
			(	"/File/New", { "command" : __menuFileNew } ),
			(	"/File/Open...", { "command" : __menuFileOpen }	),
			(	"/File/Open Recent", {}	),
			(	"/File/OpenDivider", { "divider" : True }	),
			(	"/File/Save", { "command" : __menuFileSave }	),
			(	"/File/Save As...", { "command" : __menuFileSaveAs }	),
			(	"/File/Revert To Saved", { "command" : __menuFileRevertToSaved }	),
			(	"/File/SaveDivider", { "divider" : True }	),
			(	"/File/Exit", {}	),
			
			(	"/Edit/Undo", {} 	),
			(	"/Edit/Redo", {} 	),
			(	"/Edit/UndoDivider", { "divider" : True } 	),
			(	"/Edit/Cut", {} 	),
			(	"/Edit/Copy", {} 	),
			(	"/Edit/Paste", {} 	),
			(	"/Edit/Delete", {} 	),
			(	"/Edit/Select All", {} 	),

			(	"/Layout", {} 	),
			
		]
		
	)

	def __init__( self, script ) :
	
		GafferUI.Window.__init__( self )

		self.__script = script
		
		l = GafferUI.ListContainer( GafferUI.ListContainer.Orientation.Vertical )
		l.show()
		
		m = GafferUI.MenuBar( self.menuDefinition )
		
		p = GafferUI.CompoundEditor( self.__script )
		
		l.append( m )
		l.append( p, expand=True )
		
		self.setChild( l )
		
		scriptParent = script.parent()
		if scriptParent :
			self.__scriptRemovedConnection = scriptParent.childRemovedSignal().connect( self.__scriptRemoved )

		self.gtkWidget().connect( "delete-event", self.__delete )

		self.__scriptPlugSetConnection = script.plugSetSignal().connect( self.__scriptPlugChanged )
		self.__scriptPlugDirtiedConnection = script.plugDirtiedSignal().connect( self.__scriptPlugChanged )
	
		self.__updateTitle()

	## \todo Implement setScript()
	def getScript( self ) :
	
		return self.__script

	def __delete( self, widget, event ) :
		
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
