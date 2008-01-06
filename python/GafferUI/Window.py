from Widget import Widget
import gtk

class Window( Widget ) :

	def __init__( self, title="GafferUI.Window" ) :
	
		Widget.__init__( self )
	
		self.__window = gtk.Window( gtk.WINDOW_TOPLEVEL )
		self.__window.show()
		self.setGTKWidget( self.__window )
		
		self.setTitle( title )
		
	def setTitle( self, title ) :
	
		self.__window.set_title( title )
		
	def getTitle( self ) :
	
		return self.__window.get_title()
		
	def setChild( self, child ) :
	
		oldChild = self.__window.get_child()
		if oldChild :
			self.__window.remove( oldChild )
			
		if child :
			self.__window.add( child.getGTKWidget() )

	def getChild( self ) :
	
		child = self.__window.get_child()
		if child :
			return Widget.owner( child )
			
