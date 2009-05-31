import Gaffer

## This class is used by the CompoundNumericPlugTest.
class CompoundNumericNode( Gaffer.Node ) :

	def __init__( self, name="Test", **kw ) :

		Gaffer.Node.__init__( self, name )

		self.addChild( Gaffer.V3fPlug( "p", Gaffer.Plug.Direction.In ) )

		self.setPlugs( **kw )

	def dirty( self, inputPlug ) :
	
		pass

	def typeName( self ) :
	
		return "CompoundNumericNode"
