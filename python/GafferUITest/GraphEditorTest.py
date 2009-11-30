import unittest

import IECore

import Gaffer
import GafferUI
import GafferTest

class GraphEditorTest( unittest.TestCase ) :

	def testCreateWithExistingGraph( self ) :
	
		s = Gaffer.ScriptNode()
		
		s["add1"] = GafferTest.AddNode()
		s["add2"] = GafferTest.AddNode()
		
		s["add1"]["op1"].setInput( s["add2"]["sum"] )
		
		g = GafferUI.GraphEditor( s )
	
if __name__ == "__main__":
	unittest.main()
	
