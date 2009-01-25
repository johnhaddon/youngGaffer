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

if __name__ == "__main__":
	unittest.main()
	
