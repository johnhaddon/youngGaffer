from __future__ import with_statement

import unittest

import IECore
import Gaffer

class UndoTest( unittest.TestCase ) :

	def testSetName( self ) :
	
		s = Gaffer.ScriptNode()
		self.assertRaises( Exception, s.undo )
		
		n = Gaffer.Node()
		
		s["a"] = n
		self.assertEqual( n.getName(), "a" )

		n.setName( "b" )
		self.assertEqual( n.getName(), "b" )
		self.assertRaises( Exception, s.undo )

		with Gaffer.UndoContext( s ) :
			n.setName( "c" )
			
		s.undo()
		self.assertEqual( n.getName(), "b" )
		
		s.redo()
		self.assertEqual( n.getName(), "c" )
		self.assertRaises( Exception, s.redo )

	def testSetInput( self ) :
	
		s = Gaffer.ScriptNode()
		
		n1 = Gaffer.AddNode()
		n2 = Gaffer.AddNode()
		
		s["n1"] = n1
		s["n2"] = n2
		
		with Gaffer.UndoContext( s ) :
			n1["op1"].setInput( n2["sum"] )
			
		self.assert_( n1["op1"].getInput().isSame( n2["sum"] ) )

		s.undo()
		self.assertEqual( n1["op1"].getInput(), None )
		
		s.redo()
		self.assert_( n1["op1"].getInput().isSame( n2["sum"] ) )

if __name__ == "__main__":
	unittest.main()
	
