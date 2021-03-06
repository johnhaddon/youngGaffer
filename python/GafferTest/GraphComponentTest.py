import unittest

import IECore

import Gaffer
import GafferTest

class GraphComponentTest( unittest.TestCase ) :

	def testName( self ) :
	
		c = Gaffer.GraphComponent()
		self.assertEqual( c.getName(), "GraphComponent" )
		self.assertEqual( c.fullName(), "GraphComponent" )
		
		def f( c ) :
			GraphComponentTest.name = c.getName()
			
		con = c.nameChangedSignal().connect( f )
		GraphComponentTest.name = "xxx"
		c.setName( "newName" )
		self.assertEqual( GraphComponentTest.name, "newName" )
		# slot shouldn't be called this time, as the name
		# doesn't change (it's the same value)
		c.setName( "newName" )
		self.assertEqual( self.name, "newName" )
		
		self.assertEqual( c.getName(), "newName" )
		
		child1 = Gaffer.GraphComponent()
		child2 = Gaffer.GraphComponent()
		self.assertEqual( child1.getName(), "GraphComponent" )
		self.assertEqual( child2.getName(), "GraphComponent" )
		self.assertEqual( child1.fullName(), "GraphComponent" )
		self.assertEqual( child2.fullName(), "GraphComponent" )
		
		c.addChild( child1 )
		self.assertEqual( child1.getName(), "GraphComponent" )
		self.assertEqual( child1.fullName(), "newName.GraphComponent" )
		
		con = child2.nameChangedSignal().connect( f )
		GraphComponentTest.name = "xxx"
		c.addChild( child2 )
		self.assertEqual( child2.getName(), "GraphComponent1" )
		self.assertEqual( child2.fullName(), "newName.GraphComponent1" )
		self.assertEqual( child2.relativeName( None ), "newName.GraphComponent1" )
		self.assertEqual( child2.relativeName( c ), "GraphComponent1" )
		self.assertEqual( GraphComponentTest.name, "GraphComponent1" )
		
	def testParenting( self ) :
	
		parent1 = Gaffer.GraphComponent()
		self.assert_( parent1.parent() is None )
		self.assertEqual( len( parent1.children() ), 0 )
		child1 = Gaffer.GraphComponent()
		self.assert_( child1.parent() is None )
		self.assertEqual( len( child1.children() ), 0 )
		
		parent1.addChild( child1 )
		self.assert_( parent1.parent() is None )
		self.assert_( parent1.getChild( "GraphComponent" ).isSame( child1 ) )
		self.assert_( parent1["GraphComponent"].isSame( child1 ) )
		self.assert_( child1.parent().isSame( parent1 ) )
		
		parent1.removeChild( child1 )
		self.assertEqual( parent1.children(), () )
		self.assertEqual( child1.parent(), None )
		
		self.assertRaises( RuntimeError, parent1.removeChild, child1 )
		
	def testParentingSignals( self ) :
	
		parent = Gaffer.GraphComponent()
		child = Gaffer.GraphComponent()
		
		def f( c ) :
		
			GraphComponentTest.newParent = c.parent()
		
		def ff( p, c ) :
		
			GraphComponentTest.parenting = ( p, c )
			
		c1 = child.parentChangedSignal().connect( f )	
		c2 = parent.childAddedSignal().connect( ff )
		
		GraphComponentTest.newParent = None
		GraphComponentTest.parenting = None
		parent.addChild( child )
		self.assert_( GraphComponentTest.newParent.isSame( parent ) )
		self.assert_( GraphComponentTest.parenting[0].isSame( parent ) )
		self.assert_( GraphComponentTest.parenting[1].isSame( child ) )
		
		GraphComponentTest.newParent = "xxx"
		GraphComponentTest.parenting = None
		c2 = parent.childRemovedSignal().connect( ff )
		parent.removeChild( child )
		self.assert_( GraphComponentTest.newParent is None )
		self.assert_( GraphComponentTest.parenting[0].isSame( parent ) )
		self.assert_( GraphComponentTest.parenting[1].isSame( child ) )
	
	def testReparentingDoesntSignal( self ) :
	
		"""Adding a child to a parent who already owns that child should do nothing."""
		
		parent = Gaffer.GraphComponent()
		child = Gaffer.GraphComponent()
		
		parent.addChild( child )
		self.assert_( child.parent().isSame( parent ) )
		
		GraphComponentTest.numSignals = 0
		def f( a, b=None ) :
			GraphComponentTest.numSignals += 1
			
		c1 = child.parentChangedSignal().connect( f )
		c2 = parent.childAddedSignal().connect( f )
		
		parent.addChild( child )	
		
		self.assertEqual( GraphComponentTest.numSignals, 0 )
		
	def testMany( self ) :
	
		l = []
		for i in range( 0, 100000 ) :
			l.append( Gaffer.GraphComponent() )
			
	def testDictionarySemantics( self ) :
	
		# check setitem and getitem
		p = Gaffer.GraphComponent()
		c = Gaffer.GraphComponent()
		p["c"] = c
		self.assert_( p.getChild( "c" ).isSame( c ) )
		self.assert_( p["c"].isSame( c ) )
		self.assertRaises( KeyError, p.__getitem__, "notAChild" )
		
		# check that setitem removes items with clashing names
		c2 = Gaffer.GraphComponent()
		p["c"] = c2
		self.assert_( p.getChild( "c" ).isSame( c2 ) )
		self.assert_( c2.parent().isSame( p ) )
		self.assert_( c.parent() is None )
		
		# check delitem
		c3 = Gaffer.GraphComponent()
		p["c3"] = c3
		self.assert_( p.getChild( "c3" ).isSame( c3 ) )
		self.assert_( p["c3"].isSame( c3 ) )
		self.assert_( "c3" in p )
		
		del p["c3"]
		
		self.assert_( not "c3" in p )
		
		self.assertRaises( KeyError, p.__delitem__, "xxxx" )
		
	def testUniqueNaming( self ) :
	
		p = Gaffer.GraphComponent()
		c1 = Gaffer.GraphComponent()
		c2 = Gaffer.GraphComponent()
		c3 = Gaffer.GraphComponent()
		
		c1.setName( "a" )
		c2.setName( "a" )
		c3.setName( "a" )
		
		p.addChild( c1 )
		self.assertEqual( c1.getName(), "a" )
		
		p.addChild( c2 )
		self.assertEqual( c2.getName(), "a1" )
		
		p.addChild( c3 )
		self.assertEqual( c3.getName(), "a2" )
		
		c4 = Gaffer.GraphComponent( "a1" )
		p.addChild( c4 )
		self.assertEqual( c4.getName(), "a3" )
		
	def testAncestor( self ) :
	
		a = Gaffer.ApplicationRoot()
		s = Gaffer.ScriptNode()
		a["scripts"]["one"] = s
		
		n = GafferTest.AddNode()
		s["node"] = n
		
		self.assert_( n.ancestor( Gaffer.ScriptNode.staticTypeId() ).isSame( s ) )
		self.assert_( n.ancestor( Gaffer.ApplicationRoot.staticTypeId() ).isSame( a ) )
		
	def testCommonAncestor( self ) :
	
		a = Gaffer.ApplicationRoot()
		s = Gaffer.ScriptNode()
		a["scripts"]["one"] = s
		
		s["n1"] = Gaffer.Node()
		s["n2"] = Gaffer.Node()
		
		self.assert_( s["n1"].commonAncestor( s["n2"], Gaffer.ScriptNode.staticTypeId() ).isSame( s ) )
		self.assert_( s["n2"].commonAncestor( s["n1"], Gaffer.ScriptNode.staticTypeId() ).isSame( s ) )
	
	def testRenameThenRemove( self ) :
	
		p = Gaffer.GraphComponent()
		c = Gaffer.GraphComponent()
		
		p.addChild( c )
		c.setName( "c" )
		p.removeChild( c )
		
	def testGetChildWithPath( self ) :
	
		p1 = Gaffer.GraphComponent()
		p2 = Gaffer.GraphComponent()
		p3 = Gaffer.GraphComponent()
		
		p1["p2"] = p2
		p2["p3"] = p3
		
		self.failUnless( p1.getChild( "p2.p3" ).isSame( p3 ) )
	
	def testNameConstraints( self ) :
	
		n = Gaffer.GraphComponent()
		
		for name in ( "0", "0a", "@A", "a.A", ".", "A:", "a|", "a(" ) :
			self.assertRaises( Exception, n.setName, "0" )
			
		for name in ( "hello", "_1", "brdf_0_degree_refl" ) :
			n.setName( name )
	
	def testContains( self ) :
	
		n = Gaffer.GraphComponent()
		self.failIf( "c" in n )
		n["c"] = Gaffer.GraphComponent()
		self.failUnless( "c" in n )
		
	def testIsAncestorOf( self ) :
	
		n = Gaffer.GraphComponent()
		n["c"] = Gaffer.GraphComponent()
		n["c"]["c"] = Gaffer.GraphComponent()
		n2 = Gaffer.GraphComponent()
		
		self.failUnless( n.isAncestorOf( n["c"]["c"] ) )
		self.failUnless( n.isAncestorOf( n["c"] ) )
		self.failIf( n.isAncestorOf( n ) )
		self.failIf( n2.isAncestorOf( n ) )
		self.failIf( n.isAncestorOf( n2 ) )
		
if __name__ == "__main__":
	unittest.main()
	
