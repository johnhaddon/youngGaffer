import IECore
import GafferUI
import gtk

class ScriptWindow( GafferUI.Window ) :

	## \todo We need a mechanism for different applications to add
	# different custom things to this menu and without stomping over each
	# other too
	menuDefinition = IECore.MenuDefinition(
	
		[
			(	"/File/New", {} 	),
			(	"/File/Open...", {}	),
			(	"/File/Open Recent", {}	),
			(	"/File/OpenDivider", { "divider" : True }	),
			(	"/File/Save", {}	),
			(	"/File/Save As...", {}	),
			(	"/File/Revert To Saved", {}	),
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

	def __init__( self, script, title="GafferUI.Application" ) :
	
		GafferUI.Window.__init__( self, title )

		self.__script = script
		
		l = GafferUI.ListContainer( GafferUI.ListContainer.Orientation.Vertical )
		l.show()
		
		m = GafferUI.MenuBar( self.menuDefinition )
		
		p = GafferUI.Panel()
		
		l.append( m )
		l.append( p )
		
		self.setChild( l )
		
		scriptParent = script.parent()
		if scriptParent :
			self.__scriptRemovedConnection = scriptParent.childRemovedSignal().connect( self.__scriptRemoved )

		self.gtkWidget().connect( "delete-event", self.__delete )

	def __delete( self, widget, event ) :
		
		scriptParent = self.__script.parent()
		if scriptParent :
			scriptParent.removeChild( self.__script )
			
	def __scriptRemoved( self, scriptContainer, script ) :
	
		if script.isSame( self.__script ) :
			ScriptWindow.__instances.remove( self )

	## This function provides the top level functionality for instantiating
	# the UI. Once called, new ScriptWindows will be instantiated for each
	# script added to the application, and the process will be terminated
	# when the last script is removed.
	@staticmethod
	def connect( applicationRoot ) :
	
		ScriptWindow.__scriptAddedConnection = applicationRoot.scripts.childAddedSignal().connect( ScriptWindow.__scriptAdded )
		ScriptWindow.__scriptRemovedConnection = applicationRoot.scripts.childRemovedSignal().connect( ScriptWindow.__staticScriptRemoved )

	__instances = []
	@staticmethod
	def __scriptAdded( scriptContainer, script ) :
	
		ScriptWindow.__instances.append( ScriptWindow( script ) )
		
	@staticmethod
	def __staticScriptRemoved( scriptContainer, script ) :
	
		if not len( scriptContainer.children() ) :
			
			gtk.main_quit()
		