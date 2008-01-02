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
	
		n = Gaffer.Node()
		self.assertEqual( n.getName(), "Node" )	
	
	def testOperation( self ) :
	
		class AddNode( Gaffer.Node ) :
		
			def __init__( self ) :
			
				Gaffer.Node.__init__( self )
				
				self.setName( "Add" )
				
				p1 = Gaffer.IntPlug( "op1", Gaffer.Plug.Direction.In )
				p2 = Gaffer.IntPlug( "op2", Gaffer.Plug.Direction.In )
				
				self.addChild( p1 )
				self.addChild( p2 )
				
				p3 = Gaffer.IntPlug( "sum", Gaffer.Plug.Direction.Out )
				
				self.addChild( p3 )
				
			def dirty( self, plug ) :
						
				if plug.getName()=="op1" or plug.getName()=="op2" :
					
					self.getChild( "sum" ).setDirty()
					
			def compute( self, plug ) :
			
				assert( plug.isSame( self.getChild( "sum" ) ) )
				
				plug.setValue( self.getChild("op1").getValue() + self.getChild("op2").getValue() )
		
		def dirtyCallback( plug ) :
		
			NodeTest.lastDirtied = plug.fullName()
			
		def setCallback( plug ) :
		
			NodeTest.lastSet = plug.fullName()
						
		n1 = AddNode()
		
		cb = []
		cb.append( n1.plugSetSignal().connect( setCallback ) )	
		cb.append( n1.plugDirtiedSignal().connect( dirtyCallback ) )	
		
		n1.getChild("op1").setValue( 2 )
		self.assertEqual( NodeTest.lastSet, "Add.op1" )
		self.assertEqual( n1.getChild("op1").getDirty(), False )
		self.assertEqual( n1.getChild("op2").getDirty(), False )
		self.assertEqual( n1.getChild("sum").getDirty(), True )
		self.assertEqual( NodeTest.lastDirtied, "Add.sum" )
		
		NodeTest.lastDirtied = ""
		
		n1.getChild("op2").setValue( 3 )
		self.assertEqual( NodeTest.lastSet, "Add.op2" )
		self.assertEqual( n1.getChild("op1").getDirty(), False )
		self.assertEqual( n1.getChild("op2").getDirty(), False )
		self.assertEqual( n1.getChild("sum").getDirty(), True )
		# the dirty callback shouldn't have been triggered this time,
		# as the plug was already dirty
		self.assertEqual( NodeTest.lastDirtied, "" )
		
		self.assertEqual( n1.getChild("sum").getValue(), 5 )
		self.assertEqual( NodeTest.lastSet, "Add.sum" )
		
		# connect another add node onto the output of this one
		
		n2 = AddNode()
		n2.setName( "Add2" )
		cb.append( n2.plugSetSignal().connect( setCallback ) )
		cb.append( n2.plugDirtiedSignal().connect( dirtyCallback) )
		
		n2.getChild( "op1" ).setInput( n1.getChild( "sum" ) )
		self.assertEqual( NodeTest.lastSet,"Add2.op1" )
		self.assertEqual( NodeTest.lastDirtied,"Add2.sum" )
		self.assertEqual( n2.getChild( "op1" ).getValue(), 5 )
		self.assertEqual( n2.getChild( "sum" ).getDirty(), True )
		
		self.assertEqual( n2.getChild( "sum" ).getValue(), 5 )
				
if __name__ == "__main__":
	unittest.main()
	