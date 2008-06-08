import gtk
from Gaffer import ScriptNode, Node
from GafferUI import EditorWidget, GraphGadget, Panel, GadgetWidget

class GraphEditor( EditorWidget ) :

	def __init__( self, scriptNode ) :
	
		self.__gadgetWidget = GadgetWidget( GraphGadget( scriptNode ) )
		
		EditorWidget.__init__( self, self.__gadgetWidget.gtkWidget(), scriptNode )
		
		self.__gadgetWidget.gtkWidget().connect( "button-press-event", self.makeNode )
		
	## \todo Remove me
	def makeNode( self, widget, event ) :
	
		if event.state & gtk.gdk.MOD2_MASK :
			print "MAKING NODE!"
			self.scriptNode().addChild( Node() )
			return True
	
		return False

Panel.registerContentCreator( "Graph Editor", lambda : GraphEditor( ScriptNode() ) )
