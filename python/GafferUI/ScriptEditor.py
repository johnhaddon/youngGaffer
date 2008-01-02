import gtk
import IECore
from Panel import Panel
from Gaffer import ScriptNode
from Menu import Menu
from Widget import Widget

## \todo This needs to derive off something which provides the editing context and wotnot
## \todo Output redirection of both python stderr and stdout and IECore::msg - with the option to still output to the shell as well
## \todo Fix the horizontal bar so it doesn't move unless asked - the subwindows should scroll instead
## \todo Custom right click menu with script load, save, execute file, undo, redo etc.
## \todo Standard way for users to customise all menus
class ScriptEditor( Widget ) :

	def __init__( self, scriptNode ) :
	
		Widget.__init__( self )
	
		self.__scriptNode = scriptNode
		self.__execConnection = self.__scriptNode.scriptExecutedSignal().connect( self.__execSlot )
		self.__evalConnection = self.__scriptNode.scriptEvaluatedSignal().connect( self.__evalSlot )
	
		self.__paned = gtk.VPaned()
		self.setGTKWidget( self.__paned )
		
		self.gtkOutputBuffer = gtk.TextBuffer()
		self.gtkOutputWidget = gtk.TextView( self.gtkOutputBuffer )
		self.gtkOutputWidget.set_editable( False )
		self.gtkOutputWidget.set_cursor_visible( False )
		self.gtkOutputWidget.connect( "button-press-event", self.__buttonPress )
		## \todo set the colors appropriately
		#self.gtkOutputWidget.modify_base( gtk.STATE_NORMAL, gtk.gdk.Color() )
		self.__paned.pack1( self.gtkOutputWidget, True )
		
		self.gtkInputBuffer = gtk.TextBuffer()
		self.gtkInputWidget = gtk.TextView( self.gtkInputBuffer )
		self.gtkInputWidget.connect( "key-press-event", self.__keyPress )
		self.gtkInputWidget.connect( "button-press-event", self.__buttonPress )
		
		self.__paned.pack2( self.gtkInputWidget, True )
			
		self.__paned.show_all()
	
	def __execSlot( self, scriptNode, script ) :
	
		assert( scriptNode.isSame( self.__scriptNode ) )
		self.gtkOutputBuffer.insert( self.gtkOutputBuffer.get_bounds()[1], script + "\n" )

	def __evalSlot( self, scriptNode, script, result ) :
	
		assert( scriptNode.isSame( self.__scriptNode ) )
		text = script + "\nResult : " + str( result ) + "\n"
		self.gtkOutputBuffer.insert( self.gtkOutputBuffer.get_bounds()[1], text )
		
	def __keyPress( self, widget, event ) :
	
		assert( widget is self.gtkInputWidget )
			
		if event.keyval == 65293 and event.state & gtk.gdk.CONTROL_MASK :
			
			# Ctrl-Return pressed. Execute.
			
			haveSelection = self.gtkInputBuffer.get_has_selection()
			if haveSelection :
				sel = self.gtkInputBuffer.get_selection_bounds()
				toExecute = self.gtkInputBuffer.get_text( sel[0], sel[1] )
			else :
				toExecute = self.gtkInputBuffer.get_property( "text" )
			
			try :
			
				self.__scriptNode.execute( toExecute )
				if not haveSelection :
					bounds = self.gtkInputBuffer.get_bounds()
					self.gtkInputBuffer.delete( bounds[0], bounds[1] )
			
			except Exception, e :
			
				self.gtkOutputBuffer.insert( self.gtkOutputBuffer.get_bounds()[1], str( e ) + "\n" )
			
			return True
			
		return False

	def __buttonPress( self, widget, event ) :
		
		if event.button == 3 :
		
			haveSelection = False
			if widget.get_buffer().get_selection_bounds() :
				haveSelection = True
			editable = widget.get_editable()
			clipboard = gtk.Clipboard()
						
			m = IECore.MenuDefinition()
			
			if editable :
				m.append( "/Cut", { "command" : IECore.curry( widget.get_buffer().cut_clipboard, clipboard, editable ), "active" : haveSelection } )	
			m.append( "/Copy", { "command" : IECore.curry( widget.get_buffer().copy_clipboard, clipboard ), "active" : haveSelection } )	
			if editable :
				if clipboard.wait_for_text() :
					pasteActive = True
				else :
					pasteActive = False
				m.append( "/Paste", { "command" : IECore.curry( widget.get_buffer().paste_clipboard, clipboard, None, editable ), "active" : pasteActive } )	
				m.append( "/Delete", { "command" : IECore.curry( widget.get_buffer().cut_clipboard, clipboard, editable ), "active" : haveSelection } )	
			
			m = Menu( m )
			m.popup()
			
			return True
			
		return False
		
Panel.registerContentCreator( "Script Editor", lambda : ScriptEditor( ScriptNode() ) )
