import Gaffer

## This class is used by the CompoundPlugTest.
class CompoundPlugNode( Gaffer.Node ) :

	def __init__( self, name="Test", **kw ) :
	
		Gaffer.Node.__init__( self, name )
		
		p = Gaffer.CompoundPlug( name = "p" )
		c1 = Gaffer.FloatPlug( name = "f" )
		c2 = Gaffer.StringPlug( name = "s" )
		p.addChild( c1 )
		p.addChild( c2 )
		self.addChild( p )
		
		self.setPlugs( **kw )
		
	def dirty( self, inputPlug ) :
	
		pass
		
	def typeName( self ) :
	
		return "CompoundPlugNode"
