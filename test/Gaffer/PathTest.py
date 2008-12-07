import unittest
import IECore
import Gaffer

class PathTest( unittest.TestCase ) :

	def test( self ) :
	
		self.assertRaises( ValueError, Gaffer.Path, "noStartingSlash" )

		p = Gaffer.Path( "/" )
		self.assertEqual( len( p ), 0 )
		self.assertEqual( str( p ), "/" )
		
		p = Gaffer.Path( "/a" )
		self.assertEqual( len( p ), 1 )
		self.assertEqual( p[0], "a" )
		self.assertEqual( str( p ), "/a" )

		p = Gaffer.Path( "/a//b/" )
		self.assertEqual( len( p ), 2 )
		self.assertEqual( p[0], "a" )
		self.assertEqual( p[1], "b" )
		self.assertEqual( str( p ), "/a/b" )
		
		p = Gaffer.Path( [ "a", "b" ] )
		self.assertEqual( len( p ), 2 )
		self.assertEqual( p[0], "a" )
		self.assertEqual( p[1], "b" )
		self.assertEqual( str( p ), "/a/b" )
		
		
if __name__ == "__main__":
	unittest.main()
	
