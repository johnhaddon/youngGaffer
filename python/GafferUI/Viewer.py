import gtk

import Gaffer
import GafferUI

class Viewer( GafferUI.NodeSetEditor ) :

	def __init__( self, scriptNode=None ) :
	
		GafferUI.NodeSetEditor.__init__( self, gtk.EventBox(), scriptNode )

		self.__renderableGadget = GafferUI.RenderableGadget( None )
		self.__gadgetWidget = GafferUI.GadgetWidget( self.__renderableGadget, cameraMode=GafferUI.GadgetWidget.CameraMode.Mode3D )
		
		self.gtkWidget().add( self.__gadgetWidget.gtkWidget() )
		
		self._updateFromSet()
		
	def __repr__( self ) :

		return "GafferUI.Viewer()"

	def _updateFromSet( self ) :
	
		if not hasattr( self, "_Viewer__renderableGadget" ) :
			# we're being called during construction
			return
		
		node = self.getNodeSet().lastAdded()
		if node :
			self.__plugDirtiedConnection = node.plugDirtiedSignal().connect( self.__plugDirtied )
		else :
			self.__plugDirtiedConnection = None
			
		self.__update()	
		
	def __update( self ) :
	
		renderable = None			
		node = self.getNodeSet().lastAdded()
		if node :
			renderable = node["output"].getValue()
				
		self.__renderableGadget.setRenderable( renderable )
		self.__gadgetWidget.setGadget( self.__renderableGadget )
	
	def __plugDirtied( self, plug ) :
	
		if plug.getName()=="output" :
			self.__update()
						
GafferUI.EditorWidget.registerType( "Viewer", Viewer )
