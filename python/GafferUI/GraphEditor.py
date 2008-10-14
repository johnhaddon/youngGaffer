import gtk
from Gaffer import ScriptNode, AddNode
from GafferUI import EditorWidget, GraphGadget, GadgetWidget

class GraphEditor( EditorWidget ) :

	def __init__( self, scriptNode ) :
	
		self.__gadgetWidget = GadgetWidget( GraphGadget( scriptNode ), GadgetWidget.CameraMode.Mode2D )
		
		EditorWidget.__init__( self, self.__gadgetWidget.gtkWidget(), scriptNode )
		
		self.__gadgetWidget.gtkWidget().connect( "button-press-event", self.makeNode )
		
	## \todo Remove me
	def makeNode( self, widget, event ) :
	
		if event.state & gtk.gdk.MOD2_MASK :
			print "MAKING NODE!"
			self.scriptNode().addChild( AddNode() )
			return True
	
		return False

EditorWidget.registerType( "GraphEditor", GraphEditor )
