import gtk

import IECore

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

		nodeHierarchy = IECore.RunTimeTyped.baseTypeIds( node.typeId() )
		for typeId in [ node.typeId() ] + nodeHierarchy :	
			uiBuilder = self.__uiBuilders.get( typeId, None )
			if uiBuilder is not None :
				break
						
		frame = GafferUI.Frame()
		self._column.append( frame, expand=True )
		frame.setChild( uiBuilder( node ) )
	
	__uiBuilders = {}
	@classmethod
	def registerNodeUI( cls, nodeType, uiBuilder ) :
	
		cls.__uiBuilders[nodeType] = uiBuilder	
				
GafferUI.EditorWidget.registerType( "NodeEditor", NodeEditor )
