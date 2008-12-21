import unittest

import IECore

import Gaffer

class PlugTest( unittest.TestCase ) :

	def testParenting( self ) :
	
		n = Gaffer.Node()
		p = Gaffer.Plug()
		self.assert_( p.parent() is None )
		self.assert_( p.node() is None )
		self.assert_( p.acceptsParent( n ) )
		n.addChild( p )
		self.assert_( p.parent().isSame( n ) )
		self.assert_( p.node().isSame( n ) )
	
	def testConnectionAcceptance( self ) :
	
		p1 = Gaffer.Plug( direction=Gaffer.Plug.Direction.In )
		p2 = Gaffer.Plug( direction=Gaffer.Plug.Direction.Out )
		self.assert_( p1.acceptsInput( p2 ) )
		
		p1 = Gaffer.Plug( direction=Gaffer.Plug.Direction.In )
		p2 = Gaffer.Plug( direction=Gaffer.Plug.Direction.In )
		self.assert_( p1.acceptsInput( p2 ) )
		
	def testConnection( self ) :
	
		n1 = Gaffer.Node()
		p1 = Gaffer.Plug()
		n1.addChild( p1 )
		self.assert_( p1.getInput() is None )
		self.assertEqual( p1.outputs(), () )
		
		n2 = Gaffer.Node()
		p2 = Gaffer.Plug()
		n2.addChild( p2 )
		self.assert_( p2.getInput() is None )
		self.assertEqual( p2.outputs(), () )
		
		p2.setInput( p1 )
		self.assert_( p2.getInput().isSame( p1 ) )
		self.assert_( len( p1.outputs() ), 1 )
		self.assert_( p1.outputs()[0].isSame( p2 ) )

		p2.setInput( None )
		self.assert_( p2.getInput() is None )
		self.assertEqual( p1.outputs(), () )
		
	def testConnectionSignals( self ) :
	
		def f( p ) :
		
			PlugTest.__connection = ( p, p.getInput() )
			
		n1 = Gaffer.Node()
		p1 = Gaffer.Plug( direction=Gaffer.Plug.Direction.Out )
		n1.addChild( p1 )
		
		n2 = Gaffer.Node()
		p2 = Gaffer.Plug( direction=Gaffer.Plug.Direction.In )
		n2.addChild( p2 )
		
		c = n2.plugInputChangedSignal().connect( f )
		p2.setInput( p1 )
		self.assert_( PlugTest.__connection[0].isSame( p2 ) ) 
		self.assert_( PlugTest.__connection[1].isSame( p1 ) )
		PlugTest.__connection = None
		p2.setInput( None )
		self.assert_( PlugTest.__connection[0].isSame( p2 ) ) 
		self.assert_( PlugTest.__connection[1] is None )
		
	def testDirectionality( self ) :
	
		p = Gaffer.Plug( direction=Gaffer.Plug.Direction.In )
		self.assertEqual( p.direction(), Gaffer.Plug.Direction.In )
		p = Gaffer.Plug( direction=Gaffer.Plug.Direction.Out )
		self.assertEqual( p.direction(), Gaffer.Plug.Direction.Out )
		
if __name__ == "__main__":
	unittest.main()
	
