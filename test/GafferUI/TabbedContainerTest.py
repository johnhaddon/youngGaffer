import IECore
import Gaffer
import GafferUI
import unittest

class TabbedContainerTest( unittest.TestCase ) :
	
	def test( self ) :
	
		t = GafferUI.TabbedContainer()
		self.assert_( t )
		self.assertEqual( len( t ), 0 )
		
		self.assertEqual( t.getCurrent(), None )
		
		c = GafferUI.ListContainer( GafferUI.ListContainer.Orientation.Vertical )
		
		t.append( c )
		self.assertEqual( len( t ), 1 )
		self.assert_( t[0] is c )
		self.assert_( t.getCurrent() is c )
		
if __name__ == "__main__":
	unittest.main()
	
