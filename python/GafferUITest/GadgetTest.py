import unittest

import IECore

import Gaffer
import GafferUI

class GadgetTest( unittest.TestCase ) :

	def testTransform( self ) :
	
		g = GafferUI.TextGadget( "hello" )
		self.assertEqual( g.getTransform(), IECore.M44f() )
		
		t = IECore.M44f.createScaled( IECore.V3f( 2 ) ) 
		g.setTransform( t )
		self.assertEqual( g.getTransform(), t )

		c1 = GafferUI.LinearContainer()
		c1.addChild( g )
	
		c2 = GafferUI.LinearContainer()
		c2.addChild( c1 )
		t2 = IECore.M44f.createTranslated( IECore.V3f( 1, 2, 3 ) )
		c2.setTransform( t2 )
		
		self.assertEqual( g.fullTransform(), t * t2 )
		self.assertEqual( g.fullTransform( c1 ), t )
	
if __name__ == "__main__":
	unittest.main()
	
