from __future__ import with_statement
import gtk

import IECore

import Gaffer
import GafferUI
from GafferUI import EditorWidget, GraphGadget, GadgetWidget

class GraphEditor( GafferUI.EditorWidget ) :

	def __init__( self, scriptNode=None ) :
	
		self.__gadgetWidget = GadgetWidget( cameraMode = GadgetWidget.CameraMode.Mode2D )
		self.__gadgetWidget.setBackgroundColor( IECore.Color3f( 0.07 ) )
		self.__gadgetWidget._framingBound = self.__framingBound
		
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
		
	def __framingBound( self ) :
	
		graphGadget = self.__gadgetWidget.getGadget()
		if not graphGadget :
			return IECore.Box3f()
		
		# get the bounds of the selected nodes
		scriptNode = self.getScriptNode()
		selection = scriptNode.selection()
		result = IECore.Box3f()
		for node in selection.members() :
			nodeGadget = graphGadget.nodeGadget( node )
			if nodeGadget :
				result.extendBy( nodeGadget.transformedBound( graphGadget ) )
		
		# if there were no nodes selected then use the bound of the whole
		# graph.		
		if result.isEmpty() :
			result = graphGadget.bound()
			
		# if there's still nothing then frame -10,10 so we're looking at where
		# new nodes will be appearing	
		if result.isEmpty() :
			result = IECore.Box3f( IECore.V3f( -10, -10, 0 ), IECore.V3f( 10, 10, 0 ) )
			
		# pad it a little bit so
		# it sits nicer in the frame
		result.min -= IECore.V3f( 5, 5, 0 )
		result.max += IECore.V3f( 5, 5, 0 )
				
		# now adjust the bounds so that we don't zoom in further than we want to
		boundSize = result.size()
		widgetSize = IECore.V3f( self.gtkWidget().allocation.width, self.gtkWidget().allocation.height, 0 )
		pixelsPerUnit = widgetSize / boundSize
		adjustedPixelsPerUnit = min( pixelsPerUnit.x, pixelsPerUnit.y, 10 )
		newBoundSize = widgetSize / adjustedPixelsPerUnit
		boundCenter = result.center()
		result.min = boundCenter - newBoundSize / 2.0
		result.max = boundCenter + newBoundSize / 2.0
			
		return result
		
EditorWidget.registerType( "GraphEditor", GraphEditor )
