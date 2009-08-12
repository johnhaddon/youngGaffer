import unittest

import Gaffer
import GafferUI

class SplinePlugGadgetTest( unittest.TestCase ) :

	def testSelection( self ) :
	
		g = GafferUI.SplinePlugGadget()
		
		p = Gaffer.SplineffPlug()
		p1 = p.pointPlug( p.addPoint() )
		p2 = p.pointPlug( p.addPoint() )
		
		self.failIf( p1 in g.selection() )
		self.failIf( p2 in g.selection() )
		
		# shouldn't be able to add a point to the selection if
		# the spline isn't being edited
		self.assertRaises( Exception, g.selection().add, p1 )
		
		g.splines().add( p )
		
		g.selection().add( p1 )
		self.failUnless( p1 in g.selection() )
		
		g.selection().add( p2 )
		self.failUnless( p2 in g.selection() )
		
		p.removePoint( 1 )
		self.failIf( p2 in g.selection() )
		
		g.splines().remove( p )
		
		self.failIf( p1 in g.selection() )
		
if __name__ == "__main__" :
	unittest.main()
