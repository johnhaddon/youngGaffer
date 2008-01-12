import GafferUI
import unittest
import gtk

class TestWidget( GafferUI.Widget ) :

	def __init__( self ) :
	
		GafferUI.Widget.__init__( self, gtk.Label( "hello" ) )
		self.gtkWidget().show()
		
class WindowTest( unittest.TestCase ) :

	def testTitle( self ) :
	
		w = GafferUI.Window()
		self.assertEqual( w.getTitle(), "GafferUI.Window" )
		
		w = GafferUI.Window( "myTitle" )
		self.assertEqual( w.getTitle(), "myTitle" )
		
		w.setTitle( "myOtherTitle" )
		self.assertEqual( w.getTitle(), "myOtherTitle" )
	
	def testChild( self ) :
	
		w = GafferUI.Window()
		self.assertEqual( w.getChild(), None )
		
		w.setChild( TestWidget() )
		self.assert_( not w.getChild() is None )
		self.assert_( isinstance( w.getChild(), TestWidget ) )
		
		t = TestWidget()
		w.setChild( t )
		self.assert_( w.getChild() is t )
		self.assert_( w.getChild().gtkWidget() is t.gtkWidget() )
		self.assert_( t.parent() is w )
		
		w.setChild( None )
		self.assert_( w.getChild() is None )
		self.assert_( t.parent() is None )
		# w.show()
		# gtk.main()		
		
if __name__ == "__main__":
	unittest.main()
	
