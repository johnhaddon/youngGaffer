import unittest
import IECore
import Gaffer
import GafferUI
import weakref
import gc

class WidgetSignalTest( unittest.TestCase ) :

	def test( self ) :

		w = GafferUI.TabbedContainer()

		s = GafferUI.WidgetSignal()
		self.assertEqual( s( w ), False )
		
		self.__widget = None
		def f( ww ) :
		
			self.__widget = ww
			return True
			
		c = s.connect( f )
		self.assertEqual( s( w ), True )		
		self.assert_( self.__widget is w )		
		
	def testDeletionOfConnectionDisconnects( self ) :
	
		w = GafferUI.TabbedContainer()

		s = GafferUI.WidgetSignal()
		self.assertEqual( s( w ), False )
			
		def f( ww ) :
		
			return True
			
		c = s.connect( f )

		self.assertEqual( s( w ), True )
		
		del c 

		self.assertEqual( s( w ), False )

	def testCircularRef( self ) :
	
		class A( GafferUI.TabbedContainer ) :
		
			def __init__( self ) :
			
				GafferUI.TabbedContainer.__init__( self )
				
				self.signal = GafferUI.WidgetSignal()
				
			def f( self ) :
			
				return True
				
			def ff( self, other ) :
			
				return True
				
		a = A()
		self.assertEqual( a.signal( a ), False )	
		
		a.c = a.signal.connect( A.f )
		self.assertEqual( a.signal( a ), True )
			
		w = weakref.ref( a )
		self.assert_( w() is a )
		del a
		self.assertEqual( w(), None )
		
		a2 = A()
		self.assertEqual( a2.signal( a2 ), False )	

		a2.c = a2.signal.connect( a2.ff )
		self.assertEqual( a2.signal( a2 ), True )
		
		w = weakref.ref( a2 )
		self.assert_( w() is a2 )
		del a2
		gc.collect()
		self.assertEqual( w(), None )
		
if __name__ == "__main__":
	unittest.main()
	
