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
		
	def testChildren( self ) :
	
		s = Gaffer.ScriptNode()
		n = Gaffer.Node()
		
		self.assertEqual( n.parent(), None )
		
		with Gaffer.UndoContext( s ) :
			s["n"] = n
		self.assert_( n.parent().isSame( s ) )
		s.undo()
		self.assertEqual( n.parent(), None )
		s.redo()
		self.assert_( n.parent().isSame( s ) )
		
	def testDelete( self ) :	
		
		s = Gaffer.ScriptNode()
		n1 = Gaffer.AddNode()
		n2 = Gaffer.AddNode()
		n3 = Gaffer.AddNode()
		
		s.addChild( n1 )
		s.addChild( n2 )
		s.addChild( n3 )
		
		n2["op1"].setInput( n1["sum"] )
		n2["op2"].setInput( n1["sum"] )
		n3["op1"].setInput( n2["sum"] )
		n3["op2"].setInput( n2["sum"] )
		self.assert_( n2["op1"].getInput().isSame( n1["sum"] ) )
		self.assert_( n2["op2"].getInput().isSame( n1["sum"] ) )
		self.assert_( n3["op1"].getInput().isSame( n2["sum"] ) )
		self.assert_( n3["op2"].getInput().isSame( n2["sum"] ) )
		
		with Gaffer.UndoContext( s ) :
			s.deleteNodes( Gaffer.Set( [ n2 ] ) )
			
		self.assertEqual( n2["op1"].getInput(), None )
		self.assertEqual( n2["op2"].getInput(), None )
		self.assertEqual( n3["op1"].getInput(), None )
		self.assertEqual( n3["op2"].getInput(), None )
			
		s.undo()
		
		self.assert_( n2["op1"].getInput().isSame( n1["sum"] ) )
		self.assert_( n2["op2"].getInput().isSame( n1["sum"] ) )
		self.assert_( n3["op1"].getInput().isSame( n2["sum"] ) )
		self.assert_( n3["op2"].getInput().isSame( n2["sum"] ) )
			
if __name__ == "__main__":
	unittest.main()
	
