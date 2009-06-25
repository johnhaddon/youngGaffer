import unittest

import IECore

import Gaffer
import GafferUI

class NodeGadgetTest( unittest.TestCase ) :

	def test( self ) :
	
		n = Gaffer.AddNode()
		g = GafferUI.NodeGadget.create( n )
		
		self.assertEqual( n, g.node() )
		self.assert_( g.nodule( n["op1"] ) )
		self.assert_( g.nodule( n["op2"] ) )
		self.assert_( g.nodule( n["sum"] ) )
		
	def testDynamicPlugs( self ) :
	
		n = Gaffer.AddNode()
		g = GafferUI.NodeGadget.create( n )
		
		self.assertEqual( n, g.node() )
		self.assert_( g.nodule( n["op1"] ) )
		self.assert_( g.nodule( n["op2"] ) )
		self.assert_( g.nodule( n["sum"] ) )
		
		d = Gaffer.FloatPlug()
		n["d"] = d
		
		self.assert_( g.nodule( n["op1"] ) )
		self.assert_( g.nodule( n["op2"] ) )
		self.assert_( g.nodule( n["sum"] ) )
		self.assert_( g.nodule( d ) )
		
		n.removeChild( d )
		
		self.assert_( g.nodule( n["op1"] ) )
		self.assert_( g.nodule( n["op2"] ) )
		self.assert_( g.nodule( n["sum"] ) )
		self.assert_( not g.nodule( d ) )
	
if __name__ == "__main__":
	unittest.main()
	
