import gtk

import Gaffer
import GafferUI

class NodeEditor( GafferUI.NodeSetEditor ) :

	def __init__( self, scriptNode=None ) :
	
		GafferUI.NodeSetEditor.__init__( self, gtk.EventBox(), scriptNode )
		
		self._column = GafferUI.ListContainer( GafferUI.ListContainer.Orientation.Vertical )
		self.gtkWidget().add( self._column.gtkWidget() )
		
		self._updateFromSet()
		
	def __repr__( self ) :

		return "GafferUI.NodeEditor()"

	def _updateFromSet( self ) :
	
		if not hasattr( self, "_column" ) :
			# we're being called during construction
			return
			
		del self._column[:]
		
		node = self.getNodeSet().lastAdded()
		if not node :
			return
		
		self._column.append( GafferUI.NameWidget( node ) )

		frame = GafferUI.Frame()
		self._column.append( frame, expand=True )
		uiBuilder = self.__uiBuilders.get( node.__class__, self.__defaultNodeUI )
		uiBuilder( frame, node )
	
	## \todo Some sort of NodeUI class to simplify the building process? This should
	# provide a simple mechanism for adjusting already existing uis in simple ways - for
	# instance to override the filename parts of nodes to provide fancy asset management.
	# It could also provide a simple means of having different bits of the ui disabled based
	# on callbacks from specific plugs.
	__uiBuilders = {}
	@classmethod
	def registerNodeUI( cls, nodeType, uiBuilder ) :
	
		__uiBuilders[nodeType] = uiBuilder
		
	@staticmethod
	def __defaultNodeUI( frame, node ) :
		
		column = GafferUI.ListContainer( GafferUI.ListContainer.Orientation.Vertical )
		frame.setChild( column )
		
		plugs = [ x for x in node.children() if x.isInstanceOf( Gaffer.Plug.staticTypeId() ) ]
		for plug in plugs :
		
			plugWidget = GafferUI.PlugWidget( plug )
			column.append( plugWidget )
		
GafferUI.EditorWidget.registerType( "NodeEditor", NodeEditor )
