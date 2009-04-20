import unittest

import IECore

import Gaffer

class FileSystemPathTest( unittest.TestCase ) :

	def test( self ) :
	
		p = Gaffer.FileSystemPath( __file__ )
		
		self.assert_( p.isValid() )
		self.assert_( p.isLeaf() )
	
		while len( p ) :
		
			del p[-1]
			self.assert_( p.isValid() )
			self.assert_( not p.isLeaf() )
			
	def testIsLeaf( self ) :
	
		path = Gaffer.FileSystemPath( "/this/path/doesnt/exist" )
		self.assert_( not path.isLeaf() )
		
if __name__ == "__main__":
	unittest.main()
	
