import Gaffer
import GafferUI
import gtk

class NodeEditor( GafferUI.NodeSetEditor ) :

	def __init__( self, scriptNode ) :
	
		GafferUI.NodeSetEditor.__init__( self, gtk.EventBox(), scriptNode )
		
		self._column = GafferUI.ListContainer( GafferUI.ListContainer.Orientation.Vertical )
		self.gtkWidget().add( self._column.gtkWidget() )
		
		self._updateFromSet()
		
	def _updateFromSet( self ) :
	
		if not hasattr( self, "_column" ) :
			# we're being called during construction
			return
			
		del self._column[:]
		
		## \todo We need the Set class to keep an ordering so we can get the last
		# selected fellow.
		members = list( self.getNodeSet().members() )
		if not len( members ) :
			return
		
		node = members[-1]
		self._column.append( GafferUI.NameWidget( node ) )

		plugs = [ x for x in node.children() if x.isInstanceOf( Gaffer.Plug.staticTypeId() ) ]
		for plug in plugs :
		
			plugWidget = GafferUI.PlugWidget( plug )
			self._column.append( plugWidget )
			
GafferUI.EditorWidget.registerType( "NodeEditor", NodeEditor )
