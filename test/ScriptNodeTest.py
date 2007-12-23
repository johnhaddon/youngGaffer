import unittest
import IECore
import Gaffer

class ScriptNodeTest( unittest.TestCase ) :

	def test( self ) :
	
		s = Gaffer.ScriptNode()
		self.assertEqual( s.getName(), "ScriptNode" )
		
		self.assertEqual( s.fileName.typeName(), "StringPlug" )
				
if __name__ == "__main__":
	unittest.main()
	
