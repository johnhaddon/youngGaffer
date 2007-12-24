import gtk

## \todo This needs to derive off something which provides the editing context and wotnot
class ScriptEditor :

	def __init__( self ) :
	
		self.gtkWidget = gtk.VPaned()
		
		self.gtkOutputWidget = gtk.TextView()
		self.gtkOutputWidget.set_editable( False )
		## \todo set the colors appropriately
		#self.gtkOutputWidget.modify_base( gtk.STATE_NORMAL, gtk.gdk.Color() )
		self.gtkWidget.pack1( self.gtkOutputWidget, True )
		
		self.gtkInputBuffer = gtk.TextBuffer()
		self.gtkInputWidget = gtk.TextView( self.gtkInputBuffer )
		self.gtkInputWidget.connect( "key-press-event", self.__keyPress )
		
		self.gtkWidget.pack2( self.gtkInputWidget, True )
			
		self.gtkWidget.show_all()
		
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
			
			print "EXECUTING", toExecute
			
			if not haveSelection :
				bounds = self.gtkInputBuffer.get_bounds()
				self.gtkInputBuffer.delete( bounds[0], bounds[1] )
			
			## \todo Add the execute method to the script node and use it
			
			return True
			
		return False
