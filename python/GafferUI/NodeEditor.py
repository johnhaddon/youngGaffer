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

		plugs = [ x for x in node.children() if x.isInstanceOf( Gaffer.Plug.staticTypeId() ) ]
		for plug in plugs :
		
			plugWidget = GafferUI.PlugWidget( plug )
			self._column.append( plugWidget )
			
GafferUI.EditorWidget.registerType( "NodeEditor", NodeEditor )
