import gtk

import IECore

import GafferUI

class Frame( GafferUI.ContainerWidget ) :

	def __init__( self ) :
	
		GafferUI.ContainerWidget.__init__( self, gtk.Frame() )
		self.gtkWidget().set_shadow_type( gtk.SHADOW_NONE )
		
		self.__child = None
		
	def removeChild( self, child ) :
	
		assert( child is self.__child )
		self.gtkWidget().remove( self.__child.gtkWidget() )
		self.__child = None
		
	def setChild( self, child ) :
	
		if self.__child :
			self.removeChild( self.__child )
			
		self.gtkWidget().add( child.gtkWidget() )
		self.__child = child	

	def getChild( self ) :
	
		return self.__child
