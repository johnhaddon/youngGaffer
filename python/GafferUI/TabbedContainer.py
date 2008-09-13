from ContainerWidget import ContainerWidget
import gtk

class TabbedContainer( ContainerWidget ) :

	def __init__( self ) :
	
		ContainerWidget.__init__( self, gtk.Notebook() )
		
		self.__widgets = []
				
	def append( self, child ) :
	
		oldParent = child.parent()
		if oldParent :
			oldParent.removeChild( child )
		
		self.__widgets.append( child )
		self.gtkWidget().append_page( child.gtkWidget() )
		
	def remove( self,  child ) :
	
		self.removeChild( child )
		
	def setLabel( self, child, labelText ) :
	
		assert child in self.__widgets
	
		self.gtkWidget().set_tab_label_text( child.gtkWidget(), labelText )
	
	def __getitem__( self, index ) :
	
		return self.__widgets[index]
	
	def __delitem__( self, index ) :
	
		self.removeChild( self.__widgets[index] )
	
	def __len__( self ) :
	
		return len( self.__widgets )
		
	def removeChild( self, child ) :
	
		self.__widgets.remove( child )
		self.gtkWidget().remove( child.gtkWidget() )
