from Gaffer import ScriptNode
from GafferUI import Widget

class EditorWidget( Widget ) :

	def __init__( self, gtkWidget, scriptNode ) :
	
		Widget.__init__( self, gtkWidget )
		
		if not scriptNode or not scriptNode.isInstanceOf( ScriptNode.staticTypeId() ) :
			raise TypeError( "Editor expects a ScriptNode instance.")
		
		self.__scriptNode = scriptNode
		
	def scriptNode( self ) :
	
		return self.__scriptNode
	
	@classmethod
	def types( cls ) :
	
		return cls.__namesToCreators.keys()
	
	@classmethod
	def create( cls, name, scriptNode ) :
	
		return cls.__namesToCreators[name]( scriptNode )
	
	@classmethod
	def registerType( cls, name, creator ) :
	
		cls.__namesToCreators[name] = creator
		
	__namesToCreators = {}
