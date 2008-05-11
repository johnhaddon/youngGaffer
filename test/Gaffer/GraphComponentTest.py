import unittest
import IECore
import Gaffer

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
		self.assert_( parent1.GraphComponent.isSame( child1 ) )
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
		
	def testMany( self ) :
	
		l = []
		for i in range( 0, 100000 ) :
			l.append( Gaffer.GraphComponent() )
			
	def testChildrenAsAttributes( self ) :
	
		p = Gaffer.GraphComponent()
		c = Gaffer.GraphComponent()
		cc = p.c = c
		self.assert_( p.getChild( "c" ).isSame( c ) )
		self.assert_( p.c.isSame( c ) )
		self.assert_( cc is c )
		
		# check that we can still have normal attributes not
		# as children
		s = "s"
		ss = p.s = "s"
		self.assertEqual( p.getChild( "s" ), None )
		self.assertEqual( p.s, "s" )
		self.assert_( ss is s )
		
		# we don't allow the replacing of existing children
		self.assertRaises( NameError, p.__setattr__, "c", Gaffer.GraphComponent() )
		self.assertRaises( NameError, p.__setattr__, "c", "ss" )
		# but we do allow the replacing of existing attributes
		a = "a"
		p.s = a
		self.assert_( p.s is a )
	
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
	
if __name__ == "__main__":
	unittest.main()
	
