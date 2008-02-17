import GafferUI
import unittest
import gtk

class TestWidget( GafferUI.Widget ) :

	def __init__( self, s ) :
	
		GafferUI.Widget.__init__( self, gtk.Label( s ) )
		self.gtkWidget().show()
		
		self.s = s

class ListContainerTest( unittest.TestCase ) :

	def testConstruction( self ) :
	
		c = GafferUI.ListContainer( GafferUI.ListContainer.Orientation.Vertical )
		self.assertEqual( c.orientation(), GafferUI.ListContainer.Orientation.Vertical )
		self.assertEqual( len( c ), 0 )
		
		c = GafferUI.ListContainer( GafferUI.ListContainer.Orientation.Horizontal )
		self.assertEqual( c.orientation(), GafferUI.ListContainer.Orientation.Horizontal )
		self.assertEqual( len( c ), 0 )
	
	def testItems( self ) :
	
		c = GafferUI.ListContainer( GafferUI.ListContainer.Orientation.Vertical )
		self.assertEqual( len( c ), 0 )
		
		ca = TestWidget( "a" )
		cb = TestWidget( "b" )
		cc = TestWidget( "c" )
		self.assert_( ca.parent() is None )
		self.assert_( cb.parent() is None )
		self.assert_( cc.parent() is None )
		
		c.append( ca )
		self.assertEqual( len( c ), 1 )
		self.assertEqual( c[0], ca )
		self.assert_( ca.parent() is c )
	
		c.append( cb )
		self.assertEqual( len( c ), 2 )
		self.assertEqual( c[0], ca )
		self.assertEqual( c[1], cb )
		self.assert_( ca.parent() is c )
		self.assert_( cb.parent() is c )
	
		c.append( cc )
		self.assertEqual( len( c ), 3 )
		self.assertEqual( c[0], ca )
		self.assertEqual( c[1], cb )
		self.assertEqual( c[2], cc )
		self.assert_( ca.parent() is c )
		self.assert_( cb.parent() is c )
		self.assert_( cc.parent() is c )
		
		del c[0]
		self.assertEqual( len( c ), 2 )
		self.assert_( ca.parent() is None )
		self.assert_( cb.parent() is c )
		self.assert_( cc.parent() is c )
		self.assertEqual( c[0], cb )
		self.assertEqual( c[1], cc )
		
		c.remove( cc )
		self.assertEqual( len( c ), 1 )
		self.assert_( ca.parent() is None )
		self.assert_( cb.parent() is c )
		self.assert_( cc.parent() is None )
		self.assertEqual( c[0], cb )
		
	def testReparenting( self ) :
	
		c1 = GafferUI.ListContainer( GafferUI.ListContainer.Orientation.Vertical )
		self.assertEqual( len( c1 ), 0 )
		c2 = GafferUI.ListContainer( GafferUI.ListContainer.Orientation.Vertical )
		self.assertEqual( len( c2 ), 0 )
		
		ca = TestWidget( "a" )
		self.assert_( ca.parent() is None )
		
		c1.append( ca )
		self.assert_( ca.parent() is c1 )
		self.assertEqual( len( c1 ), 1 )
		self.assertEqual( len( c2 ), 0 )
		c2.append( ca )
		self.assert_( ca.parent() is c2 )
		self.assertEqual( len( c1 ), 0 )
		self.assertEqual( len( c2 ), 1 )
		
if __name__ == "__main__":
	unittest.main()
	
