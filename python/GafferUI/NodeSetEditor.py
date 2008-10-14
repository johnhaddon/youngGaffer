import GafferUI

class NodeSetEditor( GafferUI.EditorWidget ) :

	def __init__( self, gtkWidget, scriptNode ) :
	
		GafferUI.EditorWidget.__init__( self, gtkWidget, scriptNode )
		
		self.setNodeSet( scriptNode.selection() )
		
	def setNodeSet( self, nodeSet ) :
	
		self.__nodeSet = nodeSet
		self.__memberAddedConnection = self.__nodeSet.memberAddedSignal().connect( self.__membersChanged )
		self.__memberRemovedConnection = self.__nodeSet.memberRemovedSignal().connect( self.__membersChanged )
		self._updateFromSet()
		
	def getNodeSet( self ) :
	
		return self.__nodeSet
		
	def _updateFromSet( self ) :
	
		raise NotImplementedError

	def __membersChanged( self, set, member ) :
	
		self._updateFromSet()
