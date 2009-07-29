import GafferUI

class SplineEditor( GafferUI.EditorWidget ) :

	def __init__( self, scriptNode ) :
	
		self.__column = GafferUI.ListContainer( GafferUI.ListContainer.Orientation.Vertical )
		
		GafferUI.EditorWidget.__init__( self, self.__column.gtkWidget(), scriptNode )
		
		self.__splineGadget = GafferUI.SplinePlugGadget()
		self.__selectionAddedConnection = self.__splineGadget.selection().memberAddedSignal().connect( self.__selectionChanged )
		self.__selectionRemovedConnection = self.__splineGadget.selection().memberRemovedSignal().connect( self.__selectionChanged )
		
		self.__gadgetWidget = GafferUI.GadgetWidget( self.__splineGadget, cameraMode=GafferUI.GadgetWidget.CameraMode.Mode2D )
		self.__gadgetWidget.setBackgroundColor( self._defaultBGColors[0] )
		
		self.__column.append( self.__gadgetWidget, expand=True )
		
		self.__plugWidgetRow = GafferUI.ListContainer( GafferUI.ListContainer.Orientation.Horizontal )
		self.__xPlugWidget = GafferUI.NumericPlugValueWidget( plug=None )
		self.__plugWidgetRow.append( self.__xPlugWidget, expand=True )
		self.__yPlugWidget = GafferUI.NumericPlugValueWidget( plug=None )
		self.__plugWidgetRow.append( self.__yPlugWidget, expand=True )
		
		self.__column.append( self.__plugWidgetRow )
				
	def splines( self ) :
	
		return self.__splineGadget.splines()

	def __selectionChanged( self, selection, member ) :
			
		if not selection.size() :
			self.__xPlugWidget.setPlug( None )
			self.__yPlugWidget.setPlug( None )
		else :
			selectedPlug = selection.lastAdded()
			self.__xPlugWidget.setPlug( selectedPlug["x"] )
			self.__yPlugWidget.setPlug( selectedPlug["y"] )
