from __future__ import with_statement
import gtk

import Gaffer
import GafferUI
from GafferUI import EditorWidget, GraphGadget, GadgetWidget

class GraphEditor( EditorWidget ) :

	def __init__( self, scriptNode=None ) :
	
		self.__gadgetWidget = GadgetWidget( cameraMode = GadgetWidget.CameraMode.Mode2D )
		
		EditorWidget.__init__( self, self.__gadgetWidget.gtkWidget(), scriptNode )
		
		self.__gadgetWidget.gtkWidget().connect( "button-press-event", self.__buttonPress )
	
	def setScriptNode( self, scriptNode ) :
	
		EditorWidget.setScriptNode( self, scriptNode )
		
		gadget = None
		if scriptNode :
			gadget = GraphGadget( scriptNode )
			
		self.__gadgetWidget.setGadget( gadget )
			
	def __repr__( self ) :

		return "GafferUI.GraphEditor()"	

	def __buttonPress( self, widget, event ) :
		
		if event.button==3 :
			
			# right click
			
			self.__m = GafferUI.Menu( GafferUI.NodeMenu.definition() )
			self.__m.popup( self )
						
			return True
	
		return False

EditorWidget.registerType( "GraphEditor", GraphEditor )
