import gtk

import Gaffer
import GafferUI

class SplinePlugValueWidget( GafferUI.PlugValueWidget ) :

	def __init__( self, plug ) :
	
		self.__splineWidget = GafferUI.SplineWidget()
		
		GafferUI.PlugValueWidget.__init__( self, self.__splineWidget.gtkWidget(), plug )

		self.__splineWidget.gtkWidget().connect( "button-press-event", self.__buttonPress )
		self.__splineWidget.gtkWidget().add_events( gtk.gdk.BUTTON_PRESS_MASK )

		self.__editorWindow = None

	def updateFromPlug( self ) :
	
		plug = self.getPlug()
		s = plug.getValue()
		self.__splineWidget.setSpline( s )
		
	def __buttonPress( self, widget, event ) :
	
		if event.button==1 :
						
			if not self.__editorWindow :
			
				self.__editorWindow = GafferUI.Window()
				self.__editor = GafferUI.SplineEditor( None )
				self.__editorWindow.setChild( self.__editor )
				self.__editorWindowClosedConnection = self.__editorWindow.closedSignal().connect( self.__editorWindowClosed )
			
			scriptNode = self.getPlug().ancestor( Gaffer.ScriptNode.staticTypeId() )
				
			self.ancestor( GafferUI.Window ).addChildWindow( self.__editorWindow )
			self.__editorWindow.setTitle( self.getPlug().relativeName( scriptNode ) )
			self.__editor.splines().clear()
			self.__editor.splines().add( self.getPlug() )
			
			self.__editorWindow.show()
				
	def __editorWindowClosed( self, window ) :
	
		self.__editorWindow = None

GafferUI.PlugValueWidget.registerType( Gaffer.SplineffPlug.staticTypeId(), SplinePlugValueWidget )
GafferUI.PlugValueWidget.registerType( Gaffer.SplinefColor3fPlug.staticTypeId(), SplinePlugValueWidget )
