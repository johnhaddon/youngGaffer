import gtk

## \todo This needs to derive off something which provides the editing context and wotnot
## \todo Output redirection of both python stderr and stdout and IECore::msg
## \todo Fix the horizontal bar so it doesn't move unless asked - the subwindows should scroll instead
class ScriptEditor :

	def __init__( self, scriptNode ) :
	
		self.__scriptNode = scriptNode
		self.__execConnection = self.__scriptNode.scriptExecutedSignal().connect( self.__execSlot )
		self.__evalConnection = self.__scriptNode.scriptEvaluatedSignal().connect( self.__evalSlot )
	
		self.gtkWidget = gtk.VPaned()
		
		self.gtkOutputBuffer = gtk.TextBuffer()
		self.gtkOutputWidget = gtk.TextView( self.gtkOutputBuffer )
		self.gtkOutputWidget.set_editable( False )
		## \todo set the colors appropriately
		#self.gtkOutputWidget.modify_base( gtk.STATE_NORMAL, gtk.gdk.Color() )
		self.gtkWidget.pack1( self.gtkOutputWidget, True )
		
		self.gtkInputBuffer = gtk.TextBuffer()
		self.gtkInputWidget = gtk.TextView( self.gtkInputBuffer )
		self.gtkInputWidget.connect( "key-press-event", self.__keyPress )
		
		self.gtkWidget.pack2( self.gtkInputWidget, True )
			
		self.gtkWidget.show_all()
	
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
