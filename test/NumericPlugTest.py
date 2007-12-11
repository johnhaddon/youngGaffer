import unittest
import IECore
import Gaffer

class NumericPlugTest( unittest.TestCase ) :

	def testConstructor( self ) :
	
		f = Gaffer.FloatPlug()
		self.assertEqual( f.defaultValue(), 0 )
		
		f = Gaffer.FloatPlug( direction=Gaffer.Plug.Direction.Out, defaultValue = 1,
			minValue = -1, maxValue = 10 )
			
		self.assertEqual( f.direction(), Gaffer.Plug.Direction.Out )
		self.assertEqual( f.defaultValue(), 1 )
		self.assertEqual( f.minValue(), -1 )
		self.assertEqual( f.maxValue(), 10 )
				
if __name__ == "__main__":
	unittest.main()
	
