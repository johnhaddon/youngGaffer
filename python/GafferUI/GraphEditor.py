import gtk
from Gaffer import ScriptNode, Node
from GafferUI import EditorWidget, GraphGadget, Panel, GadgetWidget

class GraphEditor( EditorWidget ) :

	def __init__( self, scriptNode ) :
	
		self.__gadgetWidget = GadgetWidget( GraphGadget( scriptNode ) )
		
		EditorWidget.__init__( self, self.__gadgetWidget.gtkWidget(), scriptNode )
		
		self.__gadgetWidget.gtkWidget().connect( "button-press-event", self.makeNode )
		
	def makeNode( self, widget, event ) :
	
		print "MAKING NODE!"
		
		self.scriptNode().addChild( Node() )
	
		return False

Panel.registerContentCreator( "Graph Editor", lambda : GraphEditor( ScriptNode() ) )
