import gtk

import Gaffer
import GafferUI
from PlugValueWidget import PlugValueWidget

class CompoundNumericPlugValueWidget( PlugValueWidget ) :

	def __init__( self, plug ) :
	
		self.__row = GafferUI.ListContainer( GafferUI.ListContainer.Orientation.Horizontal, homogeneous=True )
		
		PlugValueWidget.__init__( self, self.__row.gtkWidget(), plug )

		componentPlugs = plug.children()
		for p in componentPlugs :
			w = GafferUI.NumericPlugValueWidget( p )
			self.__row.append( w )
				
	def updateFromPlug( self ) :

		pass		
		
PlugValueWidget.registerType( Gaffer.V2fPlug.staticTypeId(), CompoundNumericPlugValueWidget )
PlugValueWidget.registerType( Gaffer.V3fPlug.staticTypeId(), CompoundNumericPlugValueWidget )
PlugValueWidget.registerType( Gaffer.V2iPlug.staticTypeId(), CompoundNumericPlugValueWidget )
PlugValueWidget.registerType( Gaffer.V3iPlug.staticTypeId(), CompoundNumericPlugValueWidget )

