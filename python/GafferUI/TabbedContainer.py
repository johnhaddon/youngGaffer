import gtk
import IECore

from Widget import Widget
from ContainerWidget import ContainerWidget

class TabbedContainer( ContainerWidget ) :

	def __init__( self ) :
	
		ContainerWidget.__init__( self, gtk.Notebook() )
		
		self.__widgets = []
		
		self._setDefaultColors( self.gtkWidget() )
		# the backgrounds of tabs which aren't currently being viewed are drawn with the active state for some reason.
		# we want them to be dark so we set the active state to a dark colour here.
		self._setColors( self.gtkWidget(), gtk.STATE_ACTIVE, IECore.Color3f( 0 ), IECore.Color3f( 0.04 ), True )
						
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
		
		labelWidget = self.gtkWidget().get_tab_label( child.gtkWidget() )
		self._setDefaultColors( labelWidget, True )
	
	def getLabel( self, child ) :
	
		return self.gtkWidget().get_tab_label_text( child.gtkWidget() )
	
	def setCurrent( self, child ) :
	
		self.gtkWidget().set_current_page( self.__widgets.index( child ) )

	def getCurrent( self ) :
	
		if not self.__widgets :
			return None
			
		return self.__widgets[ self.gtkWidget().get_current_page() ]
		
	def __getitem__( self, index ) :
	
		return self.__widgets[index]
	
	def __delitem__( self, index ) :
	
		self.removeChild( self.__widgets[index] )
	
	def __len__( self ) :
	
		return len( self.__widgets )
	
	## Implemented to always return True - otherwise empty TabbedContainers
	# are considered False and we don't really want that.
	def __nonzero__( self ) :
	
		return True
		
	def removeChild( self, child ) :
	
		self.__widgets.remove( child )
		self.gtkWidget().remove( child.gtkWidget() )
