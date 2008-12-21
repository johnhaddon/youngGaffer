import GafferUI
import gtk

class ScrolledContainer( GafferUI.ContainerWidget ) :

	def __init__( self ) :
	
		GafferUI.ContainerWidget.__init__( self, gtk.ScrolledWindow() )
		
		self.__child = None
		
	def removeChild( self, child ) :
	
		assert( child is self.__child )
		self.gtkWidget().remove( self.__child.gtkWidget() )
		self.__child = None
		
	def setChild( self, child ) :
	
		if self.__child :
			self.removeChild( self.__child )
		
		needsViewport = True
		try :
			child.gtkWidget().get_property( "hadjustment" )
			needsViewport = False
		except :
			pass
			
		if needsViewport :
			self.gtkWidget().add_with_viewport( child.gtkWidget() )
		else :
			self.gtkWidget().add( child.gtkWidget() )
		
		self.__child = child
	
	def getChild( self ) :
	
		return self.__child
