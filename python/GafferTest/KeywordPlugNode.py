import Gaffer

## This class is used by the ScriptNodeTest.
class KeywordPlugNode( Gaffer.Node ) :

	def __init__( self, name="Test", **kw ) :

		Gaffer.Node.__init__( self, name )

		# the plug name is a python keyword - we'll need the serialisation
		# format to be able to cope with that
		self.addChild( Gaffer.IntPlug( "in", Gaffer.Plug.Direction.In ) )

		self.setPlugs( **kw )

	def typeName( self ) :
	
		return "KeywordPlugNode"
