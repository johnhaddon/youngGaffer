import gtk

import GafferUI

class PathChooserWidget( GafferUI.Widget ) :

	def __init__( self, path ) :
	
		GafferUI.Widget.__init__( self, gtk.EventBox() )
		
		self.__path = path
		
		self.__column = GafferUI.ListContainer( GafferUI.ListContainer.Orientation.Vertical )
		self.gtkWidget().add( self.__column.gtkWidget() )
		
		self.__directoryListing = GafferUI.PathListingWidget( self.__path )
		self.__column.append( self.__directoryListing, True )
		
		self.__pathWidget = GafferUI.PathWidget( self.__path )
		self.__column.append( self.__pathWidget )
		
	
