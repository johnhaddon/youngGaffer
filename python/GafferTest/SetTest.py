import unittest
import weakref
import gc

import IECore

import Gaffer

class SetTest( unittest.TestCase ) :

	def testRunTimeTyped( self ) :
	
		s = Gaffer.NodeSet()
		self.assertEqual( s.typeName(), "NodeSet" )
		self.assertEqual( s.staticTypeName(), "NodeSet" )
		self.assert_( s.isInstanceOf, IECore.TypeId.RunTimeTyped )
	
	def testBasicMethods( self ) :
	
		s = Gaffer.NodeSet()
		self.assertEqual( len( s ), 0 )
		self.assertEqual( s.size(), 0 )
		
		n1 = Gaffer.Node()
		n2 = Gaffer.Node()
		
		self.assert_( n1 not in s )
		self.assert_( n2 not in s )
		
		a = s.add( n1 )
		self.assertEqual( a, True )
		self.assert_( n1 in s )
		self.assert_( not n1 not in s )
		self.assert_( n2 not in s )
		self.assertEqual( len( s ), 1 )
		self.assertEqual( s.size(), 1 )
		
		a = s.add( n1 )
		self.assertEqual( a, False )
		self.assert_( n1 in s )
		self.assert_( not n1 not in s )
		self.assert_( n2 not in s )
		self.assertEqual( len( s ), 1 )
		self.assertEqual( s.size(), 1 )
		
		a = s.add( n2 )
		self.assertEqual( a, True )
		self.assert_( n1 in s )
		self.assert_( n2 in s )
		self.assertEqual( len( s ), 2 )
		self.assertEqual( s.size(), 2 )
		
		a = s.remove( n1 )
		self.assertEqual( a, True )
		self.assert_( not n1 in s )
		self.assert_( n2 in s )
		self.assertEqual( len( s ), 1 )
		self.assertEqual( s.size(), 1 )
		
		a = s.remove( n1 )
		self.assertEqual( a, False )
		self.assert_( not n1 in s )
		self.assert_( n2 in s )
		self.assertEqual( len( s ), 1 )
		self.assertEqual( s.size(), 1 )
		
		s.clear()
		self.assert_( not n1 in s )
		self.assert_( not n2 in s )
		self.assertEqual( len( s ), 0 )
		self.assertEqual( s.size(), 0 )
	
	def testMembers( self ) :
	
		s = Gaffer.NodeSet()
		n = Gaffer.Node()
		s.add( n )
		
		m = s.members()
		self.assert_( isinstance( m, set ) )
		self.assertEqual( len( m ), 1 )
		self.assert_( n in m )
		
		w = weakref.ref( m )
		del m
		while gc.collect() :
			pass
			
		self.assert_( w() is None )
	
	def testSequencedMembers( self ) :
	
		s = Gaffer.NodeSet()
		
		l = []
		for i in range( 0, 1000 ) :
			n = Gaffer.Node()
			n.setName( str( i ) )
			l.append( n )
			s.add( n )
			
		m = s.sequencedMembers()
		self.assert_( isinstance( m, tuple ) )
		self.assertEqual( len( m ), s.size() )
		for i in range( 0, len( m ) ) :
			self.assertEqual( m[i].getName(), str( i ) )
	
	def testLastAdded( self ) :
	
		s = Gaffer.NodeSet()
		
		for i in range( 0, 1000 ) :
		
			n = Gaffer.Node()
			s.add( n )
			
			self.assert_( s.lastAdded().isSame( n ) )
		
	def testSignals( self ) :
	
		ps = set()
		def added( set, member ) :
		
			ps.add( member )
			
		def removed( set, member ) :
		
			ps.remove( member )
			
		s = Gaffer.NodeSet()
		
		c1 = s.memberAddedSignal().connect( added )
		c2 = s.memberRemovedSignal().connect( removed )
		
		n1 = Gaffer.Node()
		n2 = Gaffer.Node()
		n3 = Gaffer.Node()
		
		s.add( n1 )
		s.add( n2 )
		s.add( n3 )
		
		self.assertEqual( ps, s.members() ) 
		
		
if __name__ == "__main__":
	unittest.main()
	
