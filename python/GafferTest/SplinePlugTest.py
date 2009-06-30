import unittest

import IECore

import Gaffer

class SplinePlugTest( unittest.TestCase ) :

	def testConstructor( self ) :
	
		s = IECore.Splineff(
			IECore.CubicBasisf.catmullRom(),
			(
				( 0, 0 ),
				( 0, 0 ),
				( 0.2, 0.3 ),
				( 0.4, 0.9 ),
				( 1, 1 ),
				( 1, 1 ),
			)
		)
	
		p = Gaffer.SplineffPlug( "a", defaultValue=s )
		
		self.assertEqual( p.getValue(), s )
		
		s2 = IECore.Splineff(
			IECore.CubicBasisf.linear(),
			(
				( 1, 1 ),
				( 1, 1 ),
				( 0.2, 0.3 ),
				( 0.4, 0.9 ),
				( 0, 0 ),
				( 0, 0 ),
			)
		)
		
		p.setValue( s2 )
		
		self.assertEqual( p.getValue(), s2 ) 

if __name__ == "__main__":
	unittest.main()
	
