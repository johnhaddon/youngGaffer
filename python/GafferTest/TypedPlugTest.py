import unittest

import IECore

import Gaffer

class TypedPlugTest( unittest.TestCase ) :

	def testConstructor( self ) :
	
		s = Gaffer.StringPlug()
		self.assertEqual( s.defaultValue(), "" )
		self.assertEqual( s.getName(), "StringPlug" )
		
		s = Gaffer.StringPlug( direction=Gaffer.Plug.Direction.Out, defaultValue = "a" )
			
		self.assertEqual( s.direction(), Gaffer.Plug.Direction.Out )
		self.assertEqual( s.defaultValue(), "a" )
		
		s = Gaffer.StringPlug( defaultValue="b", name="a" )
		self.assertEqual( s.defaultValue(), "b" )
		self.assertEqual( s.getName(), "a" )
		
	def testDisconnection( self ) :
	
		p1 = Gaffer.StringPlug( direction=Gaffer.Plug.Direction.Out )
		p2 = Gaffer.StringPlug( direction=Gaffer.Plug.Direction.In )
		
		p2.setInput( p1 )
		self.assert_( p2.getInput().isSame( p1 ) )
		p2.setInput( None )
		self.assert_( p2.getInput() is None )
				
if __name__ == "__main__":
	unittest.main()
	
