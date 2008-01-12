from Widget import Widget
import gtk

class Window( Widget ) :

	def __init__( self, title="GafferUI.Window" ) :
	
		Widget.__init__( self, gtk.Window( gtk.WINDOW_TOPLEVEL ) )
		
		self.__gtkWindow = self.gtkWidget()
		self.__child = None
		
		self.setTitle( title )
		
	def setTitle( self, title ) :
	
		self.__gtkWindow.set_title( title )
		
	def getTitle( self ) :
	
		return self.__gtkWindow.get_title()
		
	def setChild( self, child ) :
	
		## \todo THIS ISN'T GOOD ENOUGH!!
		## WE CAN'T REPARENT WIDGETS
		oldChild = self.__gtkWindow.get_child()
		if oldChild :
			self.__gtkWindow.remove( oldChild )
			self.__child = None
			
		if child :
			self.__child = child
			self.__gtkWindow.add( child.gtkWidget() )

	def getChild( self ) :
	
		return self.__child	
