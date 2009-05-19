import IECore

import Gaffer

class SphereNode( Gaffer.Node ) :

	def __init__( self, name="Sphere", **kw ) :
	
		Gaffer.Node.__init__( self, name )
		
		radiusPlug = Gaffer.FloatPlug( name="radius", defaultValue=1, minValue=0 )
		self.addChild( radiusPlug )
		
		zMinPlug = Gaffer.FloatPlug( name="zMin", defaultValue=-1, minValue=-1, maxValue=1 )
		self.addChild( zMinPlug )
		
		zMaxPlug = Gaffer.FloatPlug( name="zMax", defaultValue=1, minValue=-1, maxValue=1 )
		self.addChild( zMaxPlug )
		
		thetaPlug = Gaffer.FloatPlug( name="theta", defaultValue=360, minValue=0, maxValue=360 )
		self.addChild( thetaPlug )
		
		resultPlug = Gaffer.ObjectPlug( "output", Gaffer.Plug.Direction.Out )
		self.addChild( resultPlug )
		
		self.setPlugs( **kw )

	def dirty( self, plug ) :
	
		if plug.getName() in ( "radius", "zMin", "zMax", "theta" ) :
		
			self["output"].setDirty()
			
	def compute( self, plug ) :
	
		assert( plug.isSame( self["output"] ) )
		
		result = IECore.SpherePrimitive( self["radius"].getValue(), self["zMin"].getValue(), self["zMax"].getValue(), self["theta"].getValue() )
		plug.setValue( result )

IECore.registerRunTimeTyped( SphereNode, 405001, Gaffer.Node )
