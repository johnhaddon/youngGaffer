import unittest
import sys
import StringIO

import IECore

import Gaffer
import GafferUI

class EventSignalCombinerTest( unittest.TestCase ) :

	def trueSlot( self, gadget, event ) :

		self.trueSlotCalled = True
		return True

	def falseSlot( self, gadget, event ) :

		self.falseSlotCalled = True
		return False
	
	def exceptionSlot( self, gadget, event ) :
	
		self.exceptionSlotCalled = True
		raise Exception( "oops" )
		return False
		
	def setUp( self ) :
	
		self.falseSlotCalled = False
		self.trueSlotCalled = False
		self.exceptionSlotCalled = False
			
	def testShortCutting( self ) :
	
		s = GafferUI.Gadget.ButtonSignal()
		c1 = s.connect( self.trueSlot )
		c2 = s.connect( self.falseSlot )
		
		self.assertEqual( self.trueSlotCalled, False )
		self.assertEqual( self.falseSlotCalled, False )

		self.assertEqual( s( None, GafferUI.ButtonEvent() ), True )
		
		self.assertEqual( self.trueSlotCalled, True )
		self.assertEqual( self.falseSlotCalled, False )
	
	def testNoShortCutting( self ) :
	
		s = GafferUI.Gadget.ButtonSignal()
		c1 = s.connect( self.falseSlot )
		c2 = s.connect( self.trueSlot )
		
		self.assertEqual( self.trueSlotCalled, False )
		self.assertEqual( self.falseSlotCalled, False )

		self.assertEqual( s( None, GafferUI.ButtonEvent() ), True )
		
		self.assertEqual( self.trueSlotCalled, True )
		self.assertEqual( self.falseSlotCalled, True )
	
	def testExceptionHandling( self ) :
	
		# We don't want exceptions in one slot to prevent the
		# invocation of other slots. But we do want the errors from
		# those slots being printed to stderr.
	
		s = GafferUI.Gadget.ButtonSignal()
		c1 = s.connect( self.exceptionSlot )
		c2 = s.connect( self.trueSlot )
		
		self.assertEqual( self.exceptionSlotCalled, False )
		self.assertEqual( self.trueSlotCalled, False )
		
		tmpStdErr = StringIO.StringIO()
		sys.stderr = tmpStdErr
		try :
			self.assertEqual( s( None, GafferUI.ButtonEvent() ), True )
		finally :
			sys.stderr = sys.__stderr__
		
		self.assert_( "oops" in tmpStdErr.getvalue() )	
		self.assertEqual( self.exceptionSlotCalled, True )
		self.assertEqual( self.trueSlotCalled, True )
	
if __name__ == "__main__":
	unittest.main()
	
