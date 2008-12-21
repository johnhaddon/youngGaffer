import unittest

import IECore

import Gaffer

class NumericPlugTest( unittest.TestCase ) :

	def testConstructor( self ) :
	
		f = Gaffer.FloatPlug()
		self.assertEqual( f.defaultValue(), 0 )
		self.assertEqual( f.getName(), "FloatPlug" )
		
		f = Gaffer.FloatPlug( direction=Gaffer.Plug.Direction.Out, defaultValue = 1,
			minValue = -1, maxValue = 10 )
			
		self.assertEqual( f.direction(), Gaffer.Plug.Direction.Out )
		self.assertEqual( f.defaultValue(), 1 )
		self.assertEqual( f.minValue(), -1 )
		self.assertEqual( f.maxValue(), 10 )
		
		f = Gaffer.FloatPlug( defaultValue=10, name="a" )
		self.assertEqual( f.defaultValue(), 10 )
		self.assertEqual( f.getName(), "a" )
		self.assertEqual( f.typeName(), "FloatPlug" )
	
	def testRunTimeTyping( self ) :
	
		f = Gaffer.FloatPlug()
		i = Gaffer.IntPlug()
		
		self.assert_( f.isInstanceOf( Gaffer.FloatPlug.staticTypeId() ) )			
		self.assert_( not f.isInstanceOf( Gaffer.IntPlug.staticTypeId() ) )			
		self.assert_( not i.isInstanceOf( Gaffer.FloatPlug.staticTypeId() ) )			
		self.assert_( i.isInstanceOf( Gaffer.IntPlug.staticTypeId() ) )			

if __name__ == "__main__":
	unittest.main()
	
