import gtk

import GafferUI
from ContainerWidget import ContainerWidget

class Window( ContainerWidget ) :

	def __init__( self, title="GafferUI.Window" ) :
	
		ContainerWidget.__init__( self, gtk.Window( gtk.WINDOW_TOPLEVEL ) )
		
		self.__gtkWindow = self.gtkWidget()
		self.__child = None
		
		self.setTitle( title )

		self.__closedSignal = GafferUI.WidgetSignal()
		self.gtkWidget().connect( "delete-event", self.__deleted )
		
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

	def close( self ) :
	
		self.gtkWidget().destroy()

	def closedSignal( self ) :
	
		return self.__closedSignal
	
	def __deleted( self, gtkWidget, gtkEvent ) :
	
		return self.closedSignal()( self )		
