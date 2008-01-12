import IECore
import Gaffer
import GafferUI
import IECore
import gtk
import unittest

## \todo Rejig this to remove all gtk specific code, and use only GafferUI classes instead

#window = gtk.Window( gtk.WINDOW_TOPLEVEL )
#window.set_size_request( 200, 100 )
#window.set_title( "Panel test" )
#window.connect( "delete_event", gtk.main_quit )
#
#s = GafferUI.Panel()
#assert( not s.isSplit() )
#
#window.add( s.getGTKWidget() )
#
#window.show_all()
#
#gtk.main()

class PanelTest( unittest.TestCase ) :

#	def test( self ) :
#	
#		p = GafferUI.Panel()
#		self.assertEqual( p.isSplit(), False )
#		self.assertEqual( p.getChild(), None )
#		
#		sn = Gaffer.ScriptNode()
#		
#		s1 = GafferUI.ScriptEditor( sn )
#		p.setChild( s1 )
#		self.assertEqual( p.getChild(), s1 )
#
#		p.split( GafferUI.Panel.SplitDirection.Vertical, 1 )
#		self.assertEqual( p.isSplit(), True )
#		self.assertEqual( p.subPanel( 0 ).getChild(), None )
#		self.assertEqual( p.subPanel( 1 ).getChild(), s1 )
#		
#		p.subPanel( 1 ).menuDefinition()
#		
#		p1 = p.subPanel( 1 )
#		p1.split( GafferUI.Panel.SplitDirection.Vertical, 1 )

	def testSplitAndRejoin( self ) :
	
		p = GafferUI.Panel()
				
		p.split( GafferUI.Panel.SplitDirection.Vertical, 1 )
		p.join( 0 )		


if __name__ == "__main__":
	unittest.main()
	
