import os

import IECore
import IECoreRI

import Gaffer

class SLONode( Gaffer.Node ) :

	def __init__( self, name="SLONode", inputs={}, dynamicPlugs=() ) :
	
		Gaffer.Node.__init__( self, name )
		
		self.addChild( Gaffer.StringPlug( "name" ) )
		self.addChild( Gaffer.StringPlug( "type" ) )
		self.addChild( Gaffer.CompoundPlug( "parameters" ) )
		self.addChild( Gaffer.ObjectPlug( "out", direction=Gaffer.Plug.Direction.Out ) )
		self._init( inputs, dynamicPlugs )

	def loadShader( self ) :
			
		shader = self.__readShader()
		if not shader :
			return
		
		# set the type plug			
		self["type"].setValue( shader.type )
		
		# remove old plugs which are no longer valid
		parmNames = set( shader.parameters.keys() )
		parmPlugs = self["parameters"].children()
		for parmPlug in parmPlugs :
			if parmPlug.getName() not in parmNames :
				self["parameters"].removeChild( parmPlug )
		
		# add plugs for the current parameters
		types = shader.blindData()["ri:parameterTypeHints"]
		plugTypes = {
			IECore.FloatData.staticTypeId() : Gaffer.FloatPlug,			
			IECore.V3fData.staticTypeId() : Gaffer.V3fPlug,			
			IECore.Color3fData.staticTypeId() : Gaffer.Color3fPlug,			
			IECore.StringData.staticTypeId() : Gaffer.StringPlug,			
		}
		for name, value in shader.parameters.items() :
			plug = plugTypes[value.typeId()]( name=name, flags=Gaffer.Plug.Flags.Dynamic )
			plug.setValue( value.value )
			self["parameters"][name] = plug
		
	def dirty( self, plug ) :
	
		self["out"].setDirty()
		
		if plug.getName()=="name" :
			self.loadShader()
		
	def compute( self, plug ) :
	
		assert( plug.getName()=="out" )
	
		shader = self.__readShader()
		if shader :
			for parmPlug in self["parameters"].children() :
				shader.parameters[parmPlug.getName()].value = parmPlug.getValue()

		plug.setValue( shader )

	def __readShader( self ) :
	
		sp = IECore.SearchPath( os.environ.get( "DL_SHADERS_PATH" ), ":" )
		f = sp.find( self["name"].getValue() + ".sdl" )
		if not f :
			return None
		
		reader = IECoreRI.SLOReader( f )
		return reader.read()
			
IECore.registerRunTimeTyped( SLONode, 405006, Gaffer.Node )
