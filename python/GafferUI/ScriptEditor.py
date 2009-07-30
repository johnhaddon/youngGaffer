import gtk
import IECore

from Gaffer import ScriptNode
from Menu import Menu
from Widget import Widget
from EditorWidget import EditorWidget

## \todo Output redirection of both python stderr and stdout and IECore::msg - with the option to still output to the shell as well
#		- but how do we know which script editor to output to? eh?
#			- perhaps we should only output things that this editor does and ignore all other actions?
#			- then where do messages go? a special console ui?
## \todo Custom right click menu with script load, save, execute file, undo, redo etc.
## \todo Standard way for users to customise all menus
## \todo Tab completion and popup help. rlcompleter module should be useful for tab completion. Completer( dict ) constructs a completer
# that works in a specific namespace.
class ScriptEditor( EditorWidget ) :

	def __init__( self, scriptNode=None ) :
	
		EditorWidget.__init__( self, gtk.VPaned(), scriptNode )
	
		self.__paned = self.gtkWidget()
			
		self.__gtkOutputBuffer = gtk.TextBuffer()
		self.__gtkOutputWidget = gtk.TextView( self.__gtkOutputBuffer )
		self.__gtkOutputWidget.set_editable( False )
		self.__gtkOutputWidget.set_cursor_visible( False )
		self.__gtkOutputWidget.connect( "button-press-event", self.__buttonPress )
		self.__gtkOutputScroller = gtk.ScrolledWindow()
		self.__gtkOutputScroller.set_policy( gtk.POLICY_AUTOMATIC, gtk.POLICY_AUTOMATIC )
		self.__gtkOutputScroller.add( self.__gtkOutputWidget )
		self.__paned.pack1( self.__gtkOutputScroller, True )
		
		self.__gtkInputBuffer = gtk.TextBuffer()
		self.__gtkInputWidget = gtk.TextView( self.__gtkInputBuffer )
		self.__gtkInputWidget.connect( "key-press-event", self.__keyPress )
		self.__gtkInputWidget.connect( "button-press-event", self.__buttonPress )
		self.__gtkInputScroller = gtk.ScrolledWindow()
		self.__gtkInputScroller.set_policy( gtk.POLICY_AUTOMATIC, gtk.POLICY_AUTOMATIC )
		self.__gtkInputScroller.add( self.__gtkInputWidget )
		
		self.__paned.pack2( self.__gtkInputScroller, True )
			
		self.__paned.show_all()
		
		self._setDefaultColors( self.gtkWidget(), True )
		self._setColors( self.__gtkInputWidget, gtk.STATE_NORMAL, self._textEntryFGColor, self._textEntryBGColor )

	def setScriptNode( self, scriptNode ) :
	
		EditorWidget.setScriptNode( self, scriptNode )
		
		if scriptNode :
			self.__execConnection = self.getScriptNode().scriptExecutedSignal().connect( self.__execSlot )
			self.__evalConnection = self.getScriptNode().scriptEvaluatedSignal().connect( self.__evalSlot )
		else :
			self.__execConnection = None
			self.__evalConnection = None

	def __repr__( self ) :

		return "GafferUI.ScriptEditor()"
			
	def __execSlot( self, scriptNode, script ) :
	
		assert( scriptNode.isSame( self.getScriptNode() ) )
		self.__gtkOutputBuffer.insert( self.__gtkOutputBuffer.get_bounds()[1], script + "\n" )
		self.__gtkOutputWidget.scroll_to_iter( self.__gtkOutputBuffer.get_bounds()[1], 0.0 )
		
	def __evalSlot( self, scriptNode, script, result ) :
	
		assert( scriptNode.isSame( self.getScriptNode() ) )
		text = script + "\nResult : " + str( result ) + "\n"
		self.__gtkOutputBuffer.insert( self.__gtkOutputBuffer.get_bounds()[1], text )
		self.__gtkOutputWidget.scroll_to_iter( self.__gtkOutputBuffer.get_bounds()[1], 0.0 )
		
	def __keyPress( self, widget, event ) :
	
		assert( widget is self.__gtkInputWidget )
			
		if event.keyval == 65293 and event.state & gtk.gdk.CONTROL_MASK :
			
			# Ctrl-Return pressed. Execute.
			
			haveSelection = self.__gtkInputBuffer.get_has_selection()
			if haveSelection :
				sel = self.__gtkInputBuffer.get_selection_bounds()
				toExecute = self.__gtkInputBuffer.get_text( sel[0], sel[1] )
			else :
				toExecute = self.__gtkInputBuffer.get_property( "text" )
			
			try :
			
				self.getScriptNode().execute( toExecute )
				if not haveSelection :
					bounds = self.__gtkInputBuffer.get_bounds()
					self.__gtkInputBuffer.delete( bounds[0], bounds[1] )
			
			except Exception, e :
			
				self.__gtkOutputBuffer.insert( self.__gtkOutputBuffer.get_bounds()[1], str( e ) + "\n" )
			
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
		
EditorWidget.registerType( "ScriptEditor", ScriptEditor )
