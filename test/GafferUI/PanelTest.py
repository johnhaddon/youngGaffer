import IECore
import Gaffer
import GafferUI
import IECore
import gtk
import unittest

class PanelTest( unittest.TestCase ) :

	def testChild( self ) :
	
		p = GafferUI.Panel()
		self.assertEqual( p.getChild(), None )
		self.assertEqual( p.isSplit(), False )
		p.setChild( GafferUI.ScriptEditor( Gaffer.ScriptNode() ) )
		self.assertEqual( p.isSplit(), False )
		self.assert_( isinstance( p.getChild(), GafferUI.ScriptEditor ) )
		p.setChild( None )
		self.assertEqual( p.getChild(), None )
		
		s1 = GafferUI.ScriptEditor( Gaffer.ScriptNode() )
		s2 = GafferUI.ScriptEditor( Gaffer.ScriptNode() )
		self.assert_( s1.parent() is None )
		self.assert_( s2.parent() is None )
		
		self.assertEqual( p.isSplit(), False )
		p.setChild( s1 )
		self.assertEqual( p.isSplit(), False )
		self.assert_( p.getChild() is s1 )
		self.assert_( s1.parent() is p )
		self.assert_( s2.parent() is None )
		
		self.assertEqual( p.isSplit(), False )
		p.setChild( s2 )
		self.assertEqual( p.isSplit(), False )
		self.assert_( p.getChild() is s2 )
		self.assert_( s1.parent() is None )
		self.assert_( s2.parent() is p )

	def testSplit( self ) :
	
		p = GafferUI.Panel()
		self.assertEqual( p.isSplit(), False )
		self.assertEqual( p.splitDirection(), p.SplitDirection.None )
		self.assertEqual( p.getChild(), None )
		
		p.split( p.SplitDirection.Vertical )
		self.assertEqual( p.isSplit(), True )
		self.assertEqual( p.splitDirection(), p.SplitDirection.Vertical )

		self.assertRaises( Exception, p.getChild )		
		self.assertRaises( Exception, p.split )
		self.assertRaises( Exception, p.setChild, GafferUI.ScriptEditor( Gaffer.ScriptNode() ) )
		
	def testSplitKeepingChild( self ) :
	
		p = GafferUI.Panel()
		self.assertEqual( p.isSplit(), False )
		self.assertEqual( p.splitDirection(), p.SplitDirection.None )
		self.assertEqual( p.getChild(), None )
		
		s = GafferUI.ScriptEditor( Gaffer.ScriptNode() )
		p.setChild( s )
		self.assert_( p.getChild() is s )
		self.assert_( s.parent() is p )
		self.assertEqual( p.isSplit(), False )
		self.assertEqual( p.splitDirection(), p.SplitDirection.None )
	
		p.split( p.SplitDirection.Vertical, 1 )
		self.assertEqual( p.isSplit(), True )
		self.assert_( isinstance( p.subPanel( 0 ), GafferUI.Panel ) )
		self.assert_( isinstance( p.subPanel( 1 ), GafferUI.Panel ) )
		self.assertEqual( p.subPanel( 0 ).isSplit(), False )
		self.assertEqual( p.subPanel( 1 ).isSplit(), False )	
		self.assertEqual( p.subPanel( 0 ).getChild(), None )
		self.assertEqual( p.subPanel( 1 ).getChild(), s )
		self.assert_( s.parent() is p.subPanel( 1 ) )

	def testChildTransfer( self ) :
	
		p1 = GafferUI.Panel()
		p2 = GafferUI.Panel()
		
		s = GafferUI.ScriptEditor( Gaffer.ScriptNode() )
		
		self.assertEqual( p1.getChild(), None )
		self.assertEqual( p2.getChild(), None )
		
		p1.setChild( s )
		self.assertEqual( p1.getChild(), s )
		self.assertEqual( p2.getChild(), None )
		
		p2.setChild( s )
		self.assertEqual( p1.getChild(), None )
		self.assertEqual( p2.getChild(), s )	
		
	def testSplitAndRejoin( self ) :
	
		p = GafferUI.Panel()
		self.assertEqual( p.isSplit(), False )
		self.assertEqual( p.splitDirection(), p.SplitDirection.None )
		self.assertEqual( p.getChild(), None )
				
		p.split( GafferUI.Panel.SplitDirection.Horizontal, 1 )
		self.assertEqual( p.isSplit(), True )
		self.assertEqual( p.splitDirection(), p.SplitDirection.Horizontal )	
		
		p.join( 0 )
		self.assertEqual( p.isSplit(), False )
		self.assertEqual( p.splitDirection(), p.SplitDirection.None )
		self.assertEqual( p.getChild(), None )
	
	def testSplitAndRejoinWithChild( self ) :
	
		p = GafferUI.Panel()
		s = GafferUI.ScriptEditor( Gaffer.ScriptNode() )
		p.setChild( s )
		
		p.split( p.SplitDirection.Vertical, 1 )
		self.assert_( p.isSplit() )
		self.assert_( s.parent().parent() is p )
		
		p.join( 1 )
		self.assertEqual( p.isSplit(), False )
		self.assert_( p.getChild() is s )
		self.assert_( s.parent() is p )
	
	def testSplitAndRejoinWithSplit( self ) :
	
		p = GafferUI.Panel()
		p.split( p.SplitDirection.Vertical )
		self.assert_( p.isSplit(), True )
		
		pl = p.subPanel( 0 )
		pl.split( p.SplitDirection.Horizontal )
		self.assert_( p.isSplit(), True )
		
		p.join( 0 )
		self.assert_( p.isSplit(), True )
		self.assert_( p.splitDirection(), p.SplitDirection.Horizontal )
	
if __name__ == "__main__":
	unittest.main()
	
