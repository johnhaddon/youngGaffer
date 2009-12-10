from Gaffer import ScriptNode
import GafferUI

## The EditorWidget is a base class for all Widgets which somehow display or
# manipulate a ScriptNode or its children.
class EditorWidget( GafferUI.Widget ) :

	def __init__( self, gtkWidget, scriptNode=None ) :
	
		GafferUI.Widget.__init__( self, gtkWidget )
		
		self.setScriptNode( scriptNode )
	
	def setScriptNode( self, scriptNode ) :
	
		if not ( scriptNode is None or scriptNode.isInstanceOf( ScriptNode.staticTypeId() ) ) :
			raise TypeError( "Editor expects a ScriptNode instance or None.")
		
		self.__scriptNode = scriptNode
		
	def getScriptNode( self ) :
	
		return self.__scriptNode
	
	## This must be implemented by all derived classes as it used for serialisation of layouts.
	# It is not expected that the script being edited is also serialised as part of this operation - 
	# the intention is to create a copy of the layout with no script set yet.
	def __repr__( self ) :
	
		raise NotImplementedError
	
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
