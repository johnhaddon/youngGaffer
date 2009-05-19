import IECore

import Gaffer

class ReadNode( Gaffer.Node ) :

	def __init__( self, name="Read", **kw ) :
	
		Gaffer.Node.__init__( self, name )
		
		fileNamePlug = Gaffer.StringPlug( "filename", Gaffer.Plug.Direction.In )
		self.addChild( fileNamePlug )
		
		resultPlug = Gaffer.ObjectPlug( "output", Gaffer.Plug.Direction.Out )
		self.addChild( resultPlug )
		
		self.setPlugs( **kw )

	def dirty( self, plug ) :
	
		if plug.getName()=="filename" :
		
			self["output"].setDirty()
			
	def compute( self, plug ) :
	
		assert( plug.isSame( self["output"] ) )
		
		result = None
		filename = self["filename"].getValue()
		if filename :
			
			reader = IECore.Reader.create( filename )
			if reader :
				result = reader.read()
		
		plug.setValue( result )

IECore.registerRunTimeTyped( ReadNode, 405002, Gaffer.Node )
