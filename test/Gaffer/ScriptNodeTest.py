import unittest
import IECore
import Gaffer

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

		s.execute( "child = Gaffer.Node()" )
		self.assertEqual( ScriptNodeTest.lastNode, s )
		self.assertEqual( ScriptNodeTest.lastScript, "child = Gaffer.Node()" )
		
		## \todo Implement the context stuff so this works
		self.assert_( s.child.typeName(), "Node" )
		
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
						
if __name__ == "__main__":
	unittest.main()
	
