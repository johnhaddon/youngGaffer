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
		
		n["d"] = Gaffer.FloatPlug()
		
		self.assert_( g.nodule( n["d"] ) )			
	
if __name__ == "__main__":
	unittest.main()
	
