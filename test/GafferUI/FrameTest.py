import IECore
import Gaffer
import unittest
import GafferUI

class FrameTest( unittest.TestCase ) :

	def testConstructor( self ) :
	
		g = GafferUI.RenderableGadget( IECore.MeshPrimitive.createPlane( IECore.Box2f( IECore.V2f( -1 ), IECore.V2f( 1 ) ) ) )
		f = GafferUI.Frame( g )
	
if __name__ == "__main__":
	unittest.main()
	
