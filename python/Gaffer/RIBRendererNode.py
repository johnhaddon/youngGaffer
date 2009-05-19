import os

import IECore

import Gaffer

class RIBRendererNode( Gaffer.Node ) :

	def __init__( self, name="RIBRenderer", **kw ) :
	
		Gaffer.Node.__init__( self, name )
		
		fileNamePlug = Gaffer.StringPlug( "fileName", Gaffer.Plug.Direction.In )
		self.addChild( fileNamePlug )
		
		resultPlug = Gaffer.IntPlug( "output", Gaffer.Plug.Direction.Out )
		self.addChild( resultPlug )

		self.setPlugs( **kw )
		
	def dirty( self, plug ) :
		
		if plug.getName()=="fileName" :
		
			self["output"].setDirty()
			
	def compute( self, plug ) :
	
		assert( plug.isSame( self["output"] ) )
		
		fileName = self["fileName"].getValue()
		if fileName :
		
			os.system( "renderdl \"%s\"" % fileName )
		
		## \todo Need to output useful information about what was rendered.
		# I think this should include filenames of all images, maps, ptcs
		# generated.
		plug.setValue( 0 )

IECore.registerRunTimeTyped( RIBRendererNode, 405004, Gaffer.Node )
