import gtk

import GafferUI

class Window( GafferUI.ContainerWidget ) :

	def __init__( self, title="GafferUI.Window", resizeable=True ) :
	
		GafferUI.ContainerWidget.__init__( self, gtk.Window( gtk.WINDOW_TOPLEVEL ) )
		
		self.__gtkWindow = self.gtkWidget()
		self.__child = None
		
		self.setTitle( title )
		self.setResizeable( resizeable )

		self.__closedSignal = GafferUI.WidgetSignal()
		self.gtkWidget().connect( "delete-event", self.__deleted )
		
	def show( self ) :
	
		GafferUI.ContainerWidget.show( self )
		self.gtkWidget().present()
		
	def setTitle( self, title ) :
	
		self.__gtkWindow.set_title( title )
		
	def getTitle( self ) :
	
		return self.__gtkWindow.get_title()
	
	def removeChild( self, child ) :
	
		assert( child is self.__child )
		self.__gtkWindow.remove( self.__child.gtkWidget() )
		self.__child = None
		
	def setChild( self, child ) :
	
		oldChild = self.getChild()
		if oldChild :
			self.removeChild( oldChild )
			
		if child :
			
			oldParent = child.parent()
			if oldParent :
				oldParent.removeChild( child )
							
			self.__child = child
			self.__gtkWindow.add( child.gtkWidget() )

	def getChild( self ) :
	
		return self.__child
	
	## Adding a child window causes the child to stay
	# on top of the parent at all times. This is useful for
	# preventing dialogues and the like from disappearing behind
	# the main window.
	def addChildWindow( self, childWindow ) :
	
		assert( isinstance( childWindow, Window ) )
		
		childWindow.gtkWidget().set_transient_for( self.gtkWidget() )
		
	def setResizeable( self, resizeable ) :
	
		self.gtkWidget().set_property( "resizable", resizeable )
		
	def getResizeable( self ) :
	
		return self.gtkWidget().get_property( "resizable" )

	def close( self ) :
	
		self.gtkWidget().destroy()

	def closedSignal( self ) :
	
		return self.__closedSignal
	
	def __deleted( self, gtkWidget, gtkEvent ) :
	
		return self.closedSignal()( self )		
