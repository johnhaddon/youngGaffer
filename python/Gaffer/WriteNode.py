import IECore

import Gaffer

class WriteNode( Gaffer.Node ) :

	def __init__( self, name="Write", **kw ) :
	
		Gaffer.Node.__init__( self, name )
		
		inPlug = Gaffer.ObjectPlug( "in", Gaffer.Plug.Direction.In )
		self.addChild( inPlug )
		
		fileNamePlug = Gaffer.StringPlug( "filename", Gaffer.Plug.Direction.In )
		self.addChild( fileNamePlug )
		
		resultPlug = Gaffer.StringPlug( "output", Gaffer.Plug.Direction.Out )
		self.addChild( resultPlug )
		
		self.setPlugs( **kw )

	def dirty( self, plug ) :
	
		if plug.getName()=="filename" or plug.getName()=="in" :
		
			self["output"].setDirty()
			
	def compute( self, plug ) :
	
		assert( plug.isSame( self["output"] ) )
		
		filename = self["filename"].getValue()
		if filename :
			
			writer = IECore.Writer.create( self["in"].getValue(), filename )
			if writer :
				writer.write()
		
		plug.setValue( filename )
