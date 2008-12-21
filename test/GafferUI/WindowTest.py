import unittest

import gtk
import IECore

import Gaffer
import GafferUI

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
	
	def testReparent( self ) :
	
		w1 = GafferUI.Window()
		w2 = GafferUI.Window()
		
		t = TestWidget()
		
		w1.setChild( t )
		self.assert_( t.parent() is w1 )
		self.assert_( w1.getChild() is t )
		self.assert_( w2.getChild() is None )
		self.assert_( GafferUI.Widget.owner( t.gtkWidget() ) is t )
		
		w2.setChild( t )
		self.assert_( t.parent() is w2 )
		self.assert_( w1.getChild() is None )
		self.assert_( w2.getChild() is t )
		self.assert_( GafferUI.Widget.owner( t.gtkWidget() ) is t )
		
			
if __name__ == "__main__":
	unittest.main()
	
