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
	
		p = Gaffer.Plug()
		self.assert_( n.acceptsChild( p ) )
		self.assert_( not n.acceptsParent( p ) )
		n.addChild( p )
		self.assert_( p.parent().isSame( n ) )
	
	def testNaming( self ) :
	
		## How can we fix this? Everything named "" until
		# parented to something, at which point they're named
		# typeName()? And things named "" are roots? Rubbish?
		n = Gaffer.Node()
		self.assertEqual( n.getName(), "Node" )	
		
if __name__ == "__main__":
	unittest.main()
	
