import IECore
import Gaffer
import unittest
import GafferUI

class GraphEditorTest( unittest.TestCase ) :

	def testCreateWithExistingGraph( self ) :
	
		s = Gaffer.ScriptNode()
		
		s["add1"] = Gaffer.AddNode()
		s["add2"] = Gaffer.AddNode()
		
		s["add1"]["op1"].setInput( s["add2"]["sum"] )
		
		g = GafferUI.GraphEditor( s )
	
if __name__ == "__main__":
	unittest.main()
	
