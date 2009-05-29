import unittest
import sys
import weakref
import gc

import IECore

import Gaffer
import GafferTest

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

		self.assert_( s2["a2"]["op1"].getInput().isSame( s2["a1"]["sum"] ) )
	
	def testDynamicPlugSerialisation( self ) :
	
		s1 = Gaffer.ScriptNode()
		
		s1["n1"] = Gaffer.AddNode()
		s1["n2"] = Gaffer.AddNode()
		s1["n1"]["dynamicPlug"] = Gaffer.IntPlug( flags=Gaffer.Plug.Flags.Dynamic )
		s1["n1"]["dynamicPlug"].setInput( s1["n2"]["sum"] )
		s1["n1"]["dynamicPlug2"] = Gaffer.IntPlug( flags=Gaffer.Plug.Flags.Dynamic )
		s1["n1"]["dynamicPlug2"].setValue( 100 )
		s1["n1"]["dynamicStringPlug"] = Gaffer.StringPlug( flags=Gaffer.Plug.Flags.Dynamic, value="hiThere" )
				
		s2 = Gaffer.ScriptNode()
		s2.execute( s1.serialise() )
		
		self.assert_( s2["n1"]["dynamicPlug"].getInput().isSame( s2["n2"]["sum"] ) )
		self.assertEqual( s2["n1"]["dynamicPlug2"].getValue(), 100 )
		self.assertEqual( s2["n1"]["dynamicStringPlug"].getValue(), "hiThere" )
		
	def testLifetime( self ) :
	
		s = Gaffer.ScriptNode()
		w = weakref.ref( s )
		del s
		while gc.collect() :
			pass
		IECore.RefCounted.collectGarbage()
	
		self.assertEqual( w(), None )
	
	def testSaveAndLoad( self ) :
	
		s = Gaffer.ScriptNode()
		
		s["a1"] = Gaffer.AddNode( op1=5, op2=6 )
		s["a2"] = Gaffer.AddNode( op1 = s["a1"]["sum"], op2 = 10 )
		
		s["fileName"].setValue( "/tmp/test.gfr" )
		s.save()
		
		s2 = Gaffer.ScriptNode()
		s2["fileName"].setValue( "/tmp/test.gfr" )
		s2.load()
		
		self.assert_( s2["a2"]["op1"].getInput().isSame( s2["a1"]["sum"] ) )

	def testSaveFailureHandling( self ) :
	
		s = Gaffer.ScriptNode()
		s["a1"] = Gaffer.AddNode( op1=5, op2=6 )

		s["fileName"].setValue( "/this/directory/doesnt/exist" )
		self.assertRaises( Exception, s.save )
		
	def testLoadFailureHandling( self ) :
	
		s = Gaffer.ScriptNode()
		s["a1"] = Gaffer.AddNode( op1=5, op2=6 )

		s["fileName"].setValue( "/this/file/doesnt/exist" )
		self.assertRaises( Exception, s.load )
		
	def testCopyPaste( self ) :
	
		app = Gaffer.ApplicationRoot()
		
		s1 = Gaffer.ScriptNode()
		s2 = Gaffer.ScriptNode()
		
		app["scripts"]["s1"] = s1
		app["scripts"]["s2"] = s2		
		
		n1 = Gaffer.AddNode()
		s1["n1"] = n1
		
		s1.copy()
		
		s2.paste()
		
		self.assert_( s1["n1"].isInstanceOf( Gaffer.AddNode.staticTypeId() ) )
		self.assert_( s2["n1"].isInstanceOf( Gaffer.AddNode.staticTypeId() ) )

	def testSerialisationWithKeywords( self ) :
			
		s = Gaffer.ScriptNode()
		s["n1"] = GafferTest.KeywordPlugNode()
		
		se = s.serialise()
		s2 = Gaffer.ScriptNode()
		s2.execute( se )
			
if __name__ == "__main__":
	unittest.main()
	
