from Gaffer import ScriptNode
from GafferUI import Widget

class EditorWidget( Widget ) :

	def __init__( self, gtkWidget, scriptNode=None ) :
	
		Widget.__init__( self, gtkWidget )
		
		self.setScriptNode( scriptNode )
	
	def setScriptNode( self, scriptNode ) :
	
		if not ( scriptNode is None or scriptNode.isInstanceOf( ScriptNode.staticTypeId() ) ) :
			raise TypeError( "Editor expects a ScriptNode instance or None.")
		
		self.__scriptNode = scriptNode
		
	def getScriptNode( self ) :
	
		return self.__scriptNode
	
	@classmethod
	def types( cls ) :
	
		return cls.__namesToCreators.keys()
	
	@classmethod
	def create( cls, name, scriptNode ) :
	
		return cls.__namesToCreators[name]( scriptNode = scriptNode )
	
	@classmethod
	def registerType( cls, name, creator ) :
	
		cls.__namesToCreators[name] = creator
		
	__namesToCreators = {}
