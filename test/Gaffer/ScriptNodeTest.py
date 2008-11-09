import unittest
import IECore
import Gaffer
import weakref
import sys
import gc

class ScriptNodeTest( unittest.TestCase ) :

	def setUp( self ) :
	
		ScriptNodeTest.lastNode = None
		ScriptNodeTest.lastScript = None
		ScriptNodeTest.lastResult = None

	def test( self ) :
	
		s = Gaffer.ScriptNode()
		self.assertEqual( s.getName(), "ScriptNode" )
		
		self.assertEqual( s["fileName"].typeName(), "StringPlug" )
		
	def testExecution( self ) :
	
		s = Gaffer.ScriptNode()
				
		def f( n, s ) :
			ScriptNodeTest.lastNode = n
			ScriptNodeTest.lastScript = s
			
		c = s.scriptExecutedSignal().connect( f )

		s.execute( "addChild( Gaffer.Node( 'child' ) )" )
		self.assertEqual( ScriptNodeTest.lastNode, s )
		self.assertEqual( ScriptNodeTest.lastScript, "addChild( Gaffer.Node( 'child' ) )" )
				
		self.assert_( s["child"].typeName(), "Node" )
		
	def testEvaluation( self ) :
	
		s = Gaffer.ScriptNode()
		
		def f( n, s, r ) :
			ScriptNodeTest.lastNode = n
			ScriptNodeTest.lastScript = s
			ScriptNodeTest.lastResult = r
			
		c = s.scriptEvaluatedSignal().connect( f )

		n = s.evaluate( "10 * 10" )
		self.assertEqual( n, 100 )
		self.assertEqual( ScriptNodeTest.lastNode, s )
		self.assertEqual( ScriptNodeTest.lastScript, "10 * 10" )
		self.assertEqual( ScriptNodeTest.lastResult, 100 )
				
		p = s.evaluate( "Gaffer.IntPlug()" )
		self.assertEqual( p.typeName(), "IntPlug" )
		self.assertEqual( ScriptNodeTest.lastNode, s )
		self.assertEqual( ScriptNodeTest.lastScript, "Gaffer.IntPlug()" )
		self.assert_( p.isSame( ScriptNodeTest.lastResult ) )
		del p
		del ScriptNodeTest.lastResult
		
	def testSelection( self ) :
	
		s = Gaffer.ScriptNode()
		self.assert_( isinstance( s.selection(), Gaffer.NodeSet ) )
		
	def testSerialisation( self ) :
	
		s = Gaffer.ScriptNode()
		
		s["a1"] = Gaffer.AddNode( op1=5, op2=6 )
		s["a2"] = Gaffer.AddNode( op1 = s["a1"]["sum"], op2 = 10 )
		
		s2 = Gaffer.ScriptNode()
		se = s.serialise()
		s2.execute( se )
		
	def testLifetime( self ) :
	
		s = Gaffer.ScriptNode()
		w = weakref.ref( s )
		del s
		while gc.collect() :
			pass
		IECore.RefCounted.collectGarbage()
	
		self.assertEqual( w(), None )
		
if __name__ == "__main__":
	unittest.main()
	
