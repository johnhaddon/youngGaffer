import StringIO
import unittest
import weakref
import sys
import gc

import IECore

import Gaffer

class SignalsTest( unittest.TestCase ) :

	def test( self ) :
	
		def f( a ) :
		
			return int( a )
	
		s = Gaffer.TestSignal()
		c = s.connect( f )
		self.assertEqual( c.blocked(), False )
		self.assertEqual( c.connected(), True )
		self.assert_( c.slot is f )
		
		self.assertEqual( s( 5.5 ), 5 )
		
		c.block()
		self.assertEqual( c.blocked(), True )
		c.unblock()
		self.assertEqual( c.blocked(), False )
		c.disconnect()
		self.assertEqual( c.connected(), False )
	
	def testDisconnectWhenSignalDies( self ) :
	
		def f( a ) :
			
			return int( a )
			
		s = Gaffer.TestSignal()
		c = s.connect( f )
		self.assert_( c.connected() )
		del s
		self.assert_( not c.connected() )
		
	def test2( self ) :
	
		def f( a, b ) :
		
			return a * b
	
		s = Gaffer.TestSignal2()
		c = s.connect( f )
		self.assertEqual( s( 2.0, 4.0 ), 8.0 )
	
	def testCircularRef( self ) :
	
		def default( a ) :
		
			return -1
	
		class A( IECore.V3f ) :
		
			def __init__( self ) :
			
				IECore.V3f.__init__( self )
				self.signal = Gaffer.TestSignal()
				
			def f( self, n ) :
				
				return int( n * 2 )
				
		a1 = A()
		a2 = A()
		
		# connect a signal to always return a value of -1
		defaultConnection = a2.signal.connect( default )
		self.assertEqual( a2.signal( 2 ), -1 )
		
		# connect a method in
		a1.c = a2.signal.connect( a1.f )
		self.assertEqual( a2.signal( 2 ), 4 )
		
		# connect a method of a2 to the signal on a1
		a2.c = a1.signal.connect( a2.f )
		self.assert_( a2.c.connected() )
		
		#self.assert_( a1.signal( 2 ), 4 )
		
		# just deleting a1 won't destroy it yet, as it has a
		# circular reference (a1.connection holds a1.f which
		# holds a1 which holds a1.connection)
		del a1
		self.assertEqual( a2.signal( 2 ), 4 )
		# running the garbage collector will destroy a1
		# and remove the signal
		gc.collect()
		self.assertEqual( a2.signal( 2 ), -1 )
		
		# as a1 is now dead, a2's connection to a1.signal
		# should have died.
		self.assert_( not a2.c.connected() )
			
	def deletionOfConnectionDisconnects( self ) :
	
		def default( a ) :
		
			return -1
	
		def f( a ) :
		
			return int( f * 10 )
	
		s = Gaffer.TestSignal()
		dc = s.connect( default )
		self.assertEqual( s( 1 ), -1 )
		
		c = s.connect( f )
		self.assertEqual( s( 1 ), 10 )	
	
		del c
		self.assertEqual( s( 1 ), -1 )

	def testMany( self ) :
	
		class S( IECore.V3f ) :
		
			instances = 0
		
			def __init__( self, parent ) :
			
				IECore.V3f.__init__( self )
			
				S.instances += 1
			
				self.children = []
				self.numConnections = 0
				self.signal = Gaffer.TestSignal()
				if parent :
					self.c = parent.signal.connect( self.f )
					parent.numConnections += 1
					parent.children.append( self )
					
			def f( self, a ) :
			
				r = 1
				if self.numConnections!=0 :
					r += self.signal( a )
					
				return r
					
		def build( parent, depth=0 ) :
		
			if( depth > 15 ) :
				return
			else :
				s1 = S( parent )
				s2 = S( parent )
				build( s1, depth + 1 )
				build( s2, depth + 1 )		
			
		s = S( None )
		build( s )
		
		s.signal( 1 )	
	
	## Check that Exceptions being thrown in callbacks don't cause additional references
	# to be created which would stop or delay collection. This tests a bug whereby the use
	# of PyErr_Print caused tracebacks to be stored in sys.last_traceback, which meant that
	# references to the T instance below were kept until another exception was thrown.
	def testExceptionRefCounting( self ) :
			
		class T :
		
			def __init__( self, s ) :
			
				self.connection = s.memberAddedSignal().connect( self.callback )
				
			def callback( self, s, n ) :
			
				raise Exception
	
		s = Gaffer.Set()
		t = T( s )
		w = weakref.ref( t )
		
		realStdErr = sys.stderr
		sio = StringIO.StringIO()
		try :
			sys.stderr = sio
			s.add( Gaffer.Node() )
		finally :
			sys.stderr = realStdErr
			
		del t
		while gc.collect() :
			pass
			
		self.assert_( w() is None )
		self.assert_( "Exception" in sio.getvalue() )
		
if __name__ == "__main__":
	unittest.main()
	
