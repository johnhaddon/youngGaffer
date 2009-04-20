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
		
		def dirtyCallback( plug ) :
		
			NodeTest.lastDirtied = plug.fullName()
			
		def setCallback( plug ) :
		
			NodeTest.lastSet = plug.fullName()
						
		n1 = Gaffer.AddNode()
		
		cb = []
		cb.append( n1.plugSetSignal().connect( setCallback ) )	
		cb.append( n1.plugDirtiedSignal().connect( dirtyCallback ) )	
		
		n1.getChild("op1").setValue( 2 )
		self.assertEqual( NodeTest.lastSet, "AddNode.op1" )
		self.assertEqual( n1.getChild("op1").getDirty(), False )
		self.assertEqual( n1.getChild("op2").getDirty(), False )
		self.assertEqual( n1.getChild("sum").getDirty(), True )
		self.assertEqual( NodeTest.lastDirtied, "AddNode.sum" )
		
		NodeTest.lastDirtied = ""
		
		n1.getChild("op2").setValue( 3 )
		self.assertEqual( NodeTest.lastSet, "AddNode.op2" )
		self.assertEqual( n1.getChild("op1").getDirty(), False )
		self.assertEqual( n1.getChild("op2").getDirty(), False )
		self.assertEqual( n1.getChild("sum").getDirty(), True )
		# the dirty callback shouldn't have been triggered this time,
		# as the plug was already dirty
		self.assertEqual( NodeTest.lastDirtied, "" )
		
		self.assertEqual( n1.getChild("sum").getValue(), 5 )
		self.assertEqual( NodeTest.lastSet, "AddNode.sum" )
		
		# connect another add node onto the output of this one
		
		n2 = Gaffer.AddNode()
		n2.setName( "Add2" )
		cb.append( n2.plugSetSignal().connect( setCallback ) )
		cb.append( n2.plugDirtiedSignal().connect( dirtyCallback) )
		
		n2.getChild( "op1" ).setInput( n1.getChild( "sum" ) )
		self.assertEqual( NodeTest.lastSet,"Add2.op1" )
		self.assertEqual( NodeTest.lastDirtied,"Add2.sum" )
		self.assertEqual( n2.getChild( "op1" ).getValue(), 5 )
		self.assertEqual( n2.getChild( "sum" ).getDirty(), True )
		
		self.assertEqual( n2.getChild( "sum" ).getValue(), 5 )
	
	def testScriptNode( self ) :
	
		n = Gaffer.Node()
		n2 = Gaffer.Node()
		self.assertEqual( n.scriptNode(), None )
		self.assertEqual( n2.scriptNode(), None )
		
		sn = Gaffer.ScriptNode()
		
		sn.addChild( n )
		n.addChild( n2 )
		
		self.assert_( n.scriptNode().isSame( sn ) )		
		self.assert_( n2.scriptNode().isSame( sn ) )		
	
	def testDirtyOfInputsWithConnections( self ) :
	
		n1 = Gaffer.AddNode()
		n2 = Gaffer.AddNode()
		
		n2.getChild( "op1" ).setInput( n1.getChild( "sum" ) )
		
		n1.getChild( "op1" ).setValue( 10 )
		
		self.assertEqual( n1.getChild( "sum" ).getDirty(), True )
		self.assertEqual( n2.getChild( "op1" ).getDirty(), True )
		self.assertEqual( n2.getChild( "op2" ).getDirty(), False )
		self.assertEqual( n2.getChild( "sum" ).getDirty(), True )
		
		self.assertEqual( n2.getChild( "sum" ).getValue(), 10 )
	
	def testDirtyPlugComputesSameValueAsBefore( self ) :
	
		n1 = Gaffer.AddNode( "N1" )
		n2 = Gaffer.AddNode( "N2" )
		
		n2.getChild( "op1" ).setInput( n1.getChild( "sum" ) )
		self.assertEqual( n2.getChild( "sum" ).getDirty(), False )
				
		n1.getChild( "op1" ).setValue( 1 )
		self.assertEqual( n1.getChild( "sum" ).getDirty(), True )
		self.assertEqual( n2.getChild( "op1" ).getDirty(), True )
		self.assertEqual( n2.getChild( "sum" ).getDirty(), True )
		n1.getChild( "op2" ).setValue( -1 )
		self.assertEqual( n2.getChild( "op1" ).getDirty(), True )
		self.assertEqual( n1.getChild( "sum" ).getDirty(), True )
		self.assertEqual( n2.getChild( "sum" ).getDirty(), True )
		
		self.assertEqual( n2.getChild( "sum" ).getValue(), 0 )
	
	def testExtendedConstructor( self ) :
		
		n = Gaffer.Node()
		self.assertEqual( n.getName(), "Node" )
		
		n = Gaffer.Node( "a" )
		self.assertEqual( n.getName(), "a" )
				
		self.assertRaises( Exception, Gaffer.Node, "too", "many" )
		
		n = Gaffer.AddNode( "hello", op1 = 1, op2 = 2 )
		self.assertEqual( n.getName(), "hello" )
		self.assertEqual( n["op1"].getValue(), 1 )
		self.assertEqual( n["op2"].getValue(), 2 )
		
		n2 = Gaffer.AddNode( "goodbye", op1 = n["sum"] )
		self.assert_( n2["op1"].getInput().isSame( n["sum"] ) )
									
if __name__ == "__main__":
	unittest.main()
	
