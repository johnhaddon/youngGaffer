import gtk
from IECore import Enum

from ContainerWidget import ContainerWidget

## The ListContainer holds a series of Widgets either in a column or a row.
# It attempts to provide a list like interface for manipulation of the widgets.
# \todo Support more list-like operations including slicing.
class ListContainer( ContainerWidget ) :

	Orientation = Enum.create( "Vertical", "Horizontal" )

	def __init__( self, orientation, homogeneous=False, spacing=0 ) :
	
		if orientation==self.Orientation.Vertical :
			ContainerWidget.__init__( self, gtk.VBox() )
		else :
			ContainerWidget.__init__( self, gtk.HBox() )
	
		self.gtkWidget().set_homogeneous( homogeneous )
		self.gtkWidget().set_spacing( spacing )
		
		self.__orientation = orientation
		self.__widgets = []
	
	def orientation( self ) :
	
		return self.__orientation
		
	def append( self, child, expand=False ) :
	
		oldParent = child.parent()
		if oldParent :
			oldParent.removeChild( child )
	
		self.__widgets.append( child )
		self.gtkWidget().pack_start( child.gtkWidget(), expand )	
	
	def remove( self, child ) :
	
		self.removeChild( child )
	
	def insert( self, child, index, expand=False ) :
	
		l = len( self.__widgets )
		if index > l :
			index = l
	
		oldParent = child.parent()
		if oldParent :
			oldParent.removeChild( child )
			
		self.__widgets.insert( index, child )
		self.gtkWidget().pack_start( child.gtkWidget(), expand )
		self.gtkWidget().reorder_child( child.gtkWidget(), index )
	
	def __setitem__( self, index, child ) :
	
		packing = self.gtkWidget().query_child_packing( self.__widgets[index].gtkWidget() )
		del self[index]
		self.insert( child, index )
		self.gtkWidget().set_child_packing( child.gtkWidget(), *packing )
	
	def __getitem__( self, index ) :
	
		return self.__widgets[index]
		
	def __delitem__( self, index ) :
	
		if isinstance( index, slice ) :
			indices = range( *(index.indices( len( self ) )) )
			toRemove = []
			for i in indices :
				toRemove.append( self[i] )
			for c in toRemove :
				self.gtkWidget().remove( c.gtkWidget() )
			del self.__widgets[index]
		else :
			self.removeChild( self.__widgets[index] )

	def __len__( self ) :
	
		return len( self.__widgets )
				
	def removeChild( self, child ) :
	
		self.__widgets.remove( child )
		self.gtkWidget().remove( child.gtkWidget() )
