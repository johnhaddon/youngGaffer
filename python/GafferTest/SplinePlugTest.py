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

	def testSerialisation( self ) :
	
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
	
		p = Gaffer.SplineffPlug( "a", defaultValue=s, flags=Gaffer.Plug.Flags.Dynamic )
		self.assertEqual( p.getValue(), s )
		
		sn = Gaffer.ScriptNode()
		sn["n"] = Gaffer.Node()
		sn["n"]["p"] = p
		
		se = sn.serialise()
		
		sn = Gaffer.ScriptNode()
		sn.execute( se )
		
		self.assertEqual( sn["n"]["p"].getValue(), s )

	def testPointAccess( self ) :
	
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
		p = Gaffer.SplineffPlug( "a", defaultValue=s, flags=Gaffer.Plug.Flags.Dynamic )

		self.assertEqual( p.numPoints(), 6 )
		for i in range( p.numPoints() ) :
			self.assert_( p.pointXPlug( i ).isInstanceOf( Gaffer.FloatPlug.staticTypeId() ) )
			self.assert_( p.pointYPlug( i ).isInstanceOf( Gaffer.FloatPlug.staticTypeId() ) )
			self.assert_( p.pointXPlug( i ).parent().isSame( p.pointPlug( i ) ) )
			self.assert_( p.pointYPlug( i ).parent().isSame( p.pointPlug( i ) ) )

		# accessing nonexistent points should raise exceptions
		self.assertRaises( Exception, p.pointPlug, 6 )
		self.assertRaises( Exception, p.pointXPlug, 6 )
		self.assertRaises( Exception, p.pointYPlug, 6 )

	def testPointDeletion( self ) :
	
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
		p = Gaffer.SplineffPlug( "a", defaultValue=s, flags=Gaffer.Plug.Flags.Dynamic )

		self.assertEqual( p.numPoints(), 6 )
		for i in range( p.numPoints() ) :
			self.assert_( p.pointPlug( i ) )
			self.assert_( p.pointXPlug( i ) )
			self.assert_( p.pointYPlug( i ) )
	
		p.removePoint( 0 )

		self.assertEqual( p.numPoints(), 5 )
		for i in range( p.numPoints() ) :
			self.assert_( p.pointPlug( i ) )
			self.assert_( p.pointXPlug( i ) )
			self.assert_( p.pointYPlug( i ) )
				
		p.removeChild( p.pointPlug( 0 ) )
		
		self.assertEqual( p.numPoints(), 4 )
		for i in range( p.numPoints() ) :
			self.assert_( p.pointPlug( i ) )
			self.assert_( p.pointXPlug( i ) )
			self.assert_( p.pointYPlug( i ) )
	
	def testPointTampering( self ) :
	
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
		p = Gaffer.SplineffPlug( "a", defaultValue=s, flags=Gaffer.Plug.Flags.Dynamic )
		
		del p.pointPlug( 0 )["x"]
		del p.pointPlug( 0 )["y"]
		
		self.assertRaises( Exception, p.pointXPlug, 0 )
		self.assertRaises( Exception, p.pointYPlug, 0 )
		
if __name__ == "__main__":
	unittest.main()
	
