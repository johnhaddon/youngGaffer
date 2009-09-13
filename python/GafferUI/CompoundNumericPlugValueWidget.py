import gtk

import Gaffer
import GafferUI

class CompoundNumericPlugValueWidget( GafferUI.PlugValueWidget ) :

	def __init__( self, plug ) :
	
		self.__row = GafferUI.ListContainer( GafferUI.ListContainer.Orientation.Horizontal, homogeneous=True )
		
		GafferUI.PlugValueWidget.__init__( self, self.__row.gtkWidget(), plug )

		componentPlugs = plug.children()
		for p in componentPlugs :
			w = GafferUI.NumericPlugValueWidget( p )
			self.__row.append( w )
				
	def updateFromPlug( self ) :

		pass		
		
GafferUI.PlugValueWidget.registerType( Gaffer.V2fPlug.staticTypeId(), CompoundNumericPlugValueWidget )
GafferUI.PlugValueWidget.registerType( Gaffer.V3fPlug.staticTypeId(), CompoundNumericPlugValueWidget )
GafferUI.PlugValueWidget.registerType( Gaffer.V2iPlug.staticTypeId(), CompoundNumericPlugValueWidget )
GafferUI.PlugValueWidget.registerType( Gaffer.V3iPlug.staticTypeId(), CompoundNumericPlugValueWidget )

