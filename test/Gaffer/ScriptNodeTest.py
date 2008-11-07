import unittest
import IECore
import Gaffer
import weakref
import sys
import gc

class ScriptNodeTest( unittest.TestCase ) :

#	def setUp( self ) :
#	
#		ScriptNodeTest.lastNode = None
#		ScriptNodeTest.lastScript = None
#		ScriptNodeTest.lastResult = None
#
#	def test( self ) :
#	
#		s = Gaffer.ScriptNode()
#		self.assertEqual( s.getName(), "ScriptNode" )
#		
#		self.assertEqual( s["fileName"].typeName(), "StringPlug" )
#		
#	def testExecution( self ) :
#	
#		s = Gaffer.ScriptNode()
#				
#		def f( n, s ) :
#			ScriptNodeTest.lastNode = n
#			ScriptNodeTest.lastScript = s
#			
#		c = s.scriptExecutedSignal().connect( f )
#
#		s.execute( "child = Gaffer.Node()" )
#		self.assertEqual( ScriptNodeTest.lastNode, s )
#		self.assertEqual( ScriptNodeTest.lastScript, "child = Gaffer.Node()" )
#		
#		## \todo Implement the context stuff so this works
#		self.assert_( s.child.typeName(), "Node" )
#		
#	def testEvaluation( self ) :
#	
#		s = Gaffer.ScriptNode()
#		
#		def f( n, s, r ) :
#			ScriptNodeTest.lastNode = n
#			ScriptNodeTest.lastScript = s
#			ScriptNodeTest.lastResult = r
#			
#		c = s.scriptEvaluatedSignal().connect( f )
#
#		n = s.evaluate( "10 * 10" )
#		self.assertEqual( n, 100 )
#		self.assertEqual( ScriptNodeTest.lastNode, s )
#		self.assertEqual( ScriptNodeTest.lastScript, "10 * 10" )
#		self.assertEqual( ScriptNodeTest.lastResult, 100 )
#				
#		p = s.evaluate( "Gaffer.IntPlug()" )
#		self.assertEqual( p.typeName(), "IntPlug" )
#		self.assertEqual( ScriptNodeTest.lastNode, s )
#		self.assertEqual( ScriptNodeTest.lastScript, "Gaffer.IntPlug()" )
#		self.assert_( p.isSame( ScriptNodeTest.lastResult ) )
#		del p
#		del ScriptNodeTest.lastResult
#		
#	def testSelection( self ) :
#	
#		s = Gaffer.ScriptNode()
#		self.assert_( isinstance( s.selection(), Gaffer.NodeSet ) )
#		
#	def testSerialisation( self ) :
#	
#		s = Gaffer.ScriptNode()
#		
#		s["a1"] = Gaffer.AddNode( op1=5, op2=6 )
#		s["a2"] = Gaffer.AddNode( op1 = s["a1"]["sum"], op2 = 10 )
#		
#		s2 = Gaffer.ScriptNode()
#		se = s.serialise()
#		s2.execute( se )
		
	def testLifetime( self ) :
	
		s = Gaffer.ScriptNode()
		#w = weakref.ref( s )
		print sys.getrefcount( s )
		del s
		while gc.collect() :
			pass
		IECore.RefCounted.collectGarbage()
		while gc.collect() :
			pass
		IECore.RefCounted.collectGarbage()
	
		#self.assertEqual( w(), None )
				
		# references :
		#
		# 1 from s
		# 1 from the call to getrefcount
		# 1 pointing from the c++ wrapper to s
		# 1 from the addChild method stored in __dict__
		# 
		# gc can't break the cycle because there is an extra reference from the c++ wrapper
		# wrappergarbage collector can't break the cycle because there is an extra reference
		# from __dict__
		#
		# ideas :
		#
		# * only put the methods like addChild into the globals object during calls to evaluate
		#		* slow?
		#		* does that matter?
		#		* how often is evaluate called?
		# 
		# * remove methods from the globals object when the script is unparented. that would
		#	give a window of opportunity for it to die. we could put them back if evaluate is
		#	called again later.
		#
		# * put another method in the globals? one that finds the script by some other means?
		#		* using the c++ pointer?
		# 
		# * figure out a way of having wrappergarbagecollector and gc work together to find the
		#	cycle
		
if __name__ == "__main__":
	unittest.main()
	
