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
		
		self.__pathSelectedSignal = GafferUI.WidgetSignal()

		self.__listingSelectedConnection = self.__directoryListing.pathSelectedSignal().connect( self.__pathSelected )
		self.__pathWidgetSelectedConnection = self.__pathWidget.pathSelectedSignal().connect( self.__pathSelected )

	## This signal is emitted when the user has selected a path.
	def pathSelectedSignal( self ) :
	
		return self.__pathSelectedSignal

	# This slot is connected to the pathSelectedSignals of the children and just forwards
	# them to our own pathSelectedSignal.
	def __pathSelected( self, childWidget ) :
	
		self.pathSelectedSignal()( self )
