import unittest

import Gaffer
import GafferTest

class CompoundPlugTest( unittest.TestCase ) :

	def testContructor( self ) :

		p = Gaffer.CompoundPlug()
		self.assertEqual( p.getName(), "CompoundPlug" )
		self.assertEqual( p.direction(), Gaffer.Plug.Direction.In )

		p = Gaffer.V3fPlug( name="b", direction=Gaffer.Plug.Direction.Out )
		self.assertEqual( p.getName(), "b" )
		self.assertEqual( p.direction(), Gaffer.Plug.Direction.Out )

	def testSerialisation( self ) :
	
		s = Gaffer.ScriptNode()
		s["n1"] = GafferTest.CompoundPlugNode()
		s["n2"] = GafferTest.CompoundPlugNode()
		
		s["n1"]["p"]["f"].setValue( 10 )
		s["n1"]["p"]["s"].setInput( s["n2"]["p"]["s"] )
		
		ss = s.serialise()
				
		s = Gaffer.ScriptNode()
		s.execute( ss )
		
		self.assertEqual( s["n1"]["p"]["f"].getValue(), 10 )
		self.failUnless( s["n1"]["p"]["s"].getInput().isSame( s["n2"]["p"]["s"] ) )
		
	def testDynamicSerialisation( self ) :
	
		s = Gaffer.ScriptNode()
		s["n1"] = Gaffer.Node()
		s["n1"]["p"] = Gaffer.CompoundPlug( flags = Gaffer.Plug.Flags.Dynamic )
		s["n1"]["p"]["f"] = Gaffer.FloatPlug( flags = Gaffer.Plug.Flags.Dynamic )
		s["n1"]["p"]["f"].setValue( 10 )
		
		ss = s.serialise()
				
		s = Gaffer.ScriptNode()
		s.execute( ss )
		
		self.assertEqual( s["n1"]["p"]["f"].getValue(), 10 )
		
if __name__ == "__main__":
	unittest.main()
	
