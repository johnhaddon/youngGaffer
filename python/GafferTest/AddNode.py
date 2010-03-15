import IECore
import Gaffer

class AddNode( Gaffer.Node ) :
		
	def __init__( self, name="AddNode", inputs={}, dynamicPlugs=() ) :

		Gaffer.Node.__init__( self, name )

		p1 = Gaffer.IntPlug( "op1", Gaffer.Plug.Direction.In )
		p2 = Gaffer.IntPlug( "op2", Gaffer.Plug.Direction.In )

		self.addChild( p1 )
		self.addChild( p2 )

		p3 = Gaffer.IntPlug( "sum", Gaffer.Plug.Direction.Out )

		self.addChild( p3 )
		
		self._init( inputs, dynamicPlugs )

	def dirty( self, plug ) :

		if plug.getName()=="op1" or plug.getName()=="op2" :

			self.getChild( "sum" ).setDirty()

	def compute( self, plug ) :

		assert( plug.isSame( self.getChild( "sum" ) ) )

		plug.setValue( self.getChild("op1").getValue() + self.getChild("op2").getValue() )

IECore.registerRunTimeTyped( AddNode )
