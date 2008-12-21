import unittest

import IECore

import Gaffer
import GafferUI

class NodeGadgetTest( unittest.TestCase ) :

	def test( self ) :
	
		n = Gaffer.AddNode()
		g = GafferUI.NodeGadget( n )
		
		self.assertEqual( n, g.node() )
		self.assert_( g.nodule( n["op1"] ) )
		self.assert_( g.nodule( n["op2"] ) )
		self.assert_( g.nodule( n["sum"] ) )
	
if __name__ == "__main__":
	unittest.main()
	
