import IECore

import Gaffer

class GroupNode( Gaffer.Node ) :

	def __init__( self, name="Group", inputs={}, dynamicPlugs=() ) :
	
		Gaffer.Node.__init__( self, name )
		
		self.addChild( Gaffer.V3fPlug( "translate" ) )
		self.addChild( Gaffer.V3fPlug( "rotate" ) )
		self.addChild( Gaffer.V3fPlug( "scale", defaultValue=IECore.V3f( 1 ) ) )

		resultPlug = Gaffer.ObjectPlug( "output", Gaffer.Plug.Direction.Out )
		self.addChild( resultPlug )
		
		self._init( inputs, dynamicPlugs )
		
		self.__inputChangedConnection = self.plugInputChangedSignal().connect( self.__plugInputChanged )

		self.__addAndRemoveInputs()

	def matrix( self ) :
	
		translate = IECore.M44f.createTranslated( self["translate"].getValue() )
		rotate = IECore.Eulerf( self["rotate"].getValue(), IECore.Eulerf.Order.XYZ, IECore.Eulerf.InputLayout.XYZLayout )
		rotate = rotate.toMatrix44()
		scale = IECore.M44f.createScaled( self["scale"].getValue() )
		transforms = {
			"t" : translate,
			"r" : rotate,
			"s" : scale,
		}
		transform = IECore.M44f()
		for m in ( "trs" ) :
			transform = transform * transforms[m]
		
		return transform

	def dirty( self, plug ) :
	
		self["output"].setDirty()
				
	def compute( self, plug ) :
	
		assert( plug.isSame( self["output"] ) )
	
		inputs = self.__inputs()
		result = IECore.Group()
		for i in inputs :
			if i.getInput() :
				v = i.getValue()
				if v.isInstanceOf( IECore.VisibleRenderable.staticTypeId() ) :
					result.addChild( v )
				elif v.isInstanceOf( IECore.StateRenderable.staticTypeId() ) :
					result.addState( v )
		
		result.setTransform( IECore.MatrixTransform( self.matrix() ) )			
				
		plug.setValue( result )

	def __plugInputChanged( self, plug ) :
	
		self.__addAndRemoveInputs()
	
	def __inputs( self ) :
	
		inputs = self.children()
		inputs = [ x for x in inputs if x.isInstanceOf( Gaffer.ObjectPlug.staticTypeId() ) and x.direction()==Gaffer.Plug.Direction.In ]
		return inputs
		
	def __addAndRemoveInputs( self ) :
	
		inputs = self.children()
		inputs = [ x for x in inputs if x.isInstanceOf( Gaffer.ObjectPlug.staticTypeId() ) and x.direction()==Gaffer.Plug.Direction.In ]
		lastConnected = -1
		for i in range( 0, len( inputs ) ) :
			if inputs[i].getInput() is not None :
				lastConnected = i
						
		if lastConnected == len( inputs ) - 1 :
			newInput = Gaffer.ObjectPlug( "in1", flags = Gaffer.Plug.Flags.Dynamic )
			self.addChild( newInput )
		else :
			for plug in inputs[lastConnected + 2:] :
				self.removeChild( plug ) 
				
IECore.registerRunTimeTyped( GroupNode, 405005, Gaffer.Node )
