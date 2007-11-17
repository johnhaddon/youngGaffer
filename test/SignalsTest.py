import unittest
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
		
	## \todo
	# test circular references
	# test deletion of a connection causes disconnect
			
if __name__ == "__main__":
	unittest.main()
	
