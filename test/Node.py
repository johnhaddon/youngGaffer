import unittest
import IECore
import Gaffer

class NodeTest( unittest.TestCase ) :

	def testParenting( self ) :
	
		c = Gaffer.GraphComponent()
		n = Gaffer.Node()
		self.assertEqual( n.acceptsParent( c ), False )
		self.assertRaises( RuntimeError, c.addChild, n )
	
		n2 = Gaffer.Node()
		self.assertEqual( n.acceptsParent( n2 ), True )
		n2.addChild( n )
	
	## \todo 
	#!!!! TEST SIGNALS AND WOTNOT!!!!
	#!!!! AND PLUGS AS CHILDREN !!!
		
if __name__ == "__main__":
	unittest.main()
	
