import unittest

import IECore

import Gaffer

class TypedObjectPlugTest( unittest.TestCase ) :

	def testSerialisation( self ) :
	
		s = Gaffer.ScriptNode()
		s["n"] = Gaffer.Node()
		s["n"]["t"] = Gaffer.ObjectPlug( "hello", flags = Gaffer.Plug.Flags.Dynamic )
		
		se = s.serialise()
		
		s2 = Gaffer.ScriptNode()
		s2.execute( se )
		
		self.failUnless( s2["n"]["t"].isInstanceOf( Gaffer.ObjectPlug.staticTypeId() ) )		

	def testSerialisationWithConnection( self ) :
	
		s = Gaffer.ScriptNode()
		s["n"] = Gaffer.Node()
		s["n"]["t"] = Gaffer.ObjectPlug( "hello", flags = Gaffer.Plug.Flags.Dynamic )
		
		s["n2"] = Gaffer.Node()
		s["n2"]["t2"] = Gaffer.ObjectPlug( "hello", flags = Gaffer.Plug.Flags.Dynamic, direction=Gaffer.Plug.Direction.Out )
		
		s["n"]["t"].setInput( s["n2"]["t2"] )
		
		se = s.serialise()
				
		s2 = Gaffer.ScriptNode()
		s2.execute( se )
		
		self.failUnless( s2["n"]["t"].getInput().isSame( s2["n2"]["t2"] ) )		
				
if __name__ == "__main__":
	unittest.main()
	
