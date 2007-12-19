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
				
if __name__ == "__main__":
	unittest.main()
	
