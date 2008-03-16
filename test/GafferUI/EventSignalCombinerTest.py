import IECore
import Gaffer
import GafferUI
import unittest
import gtk

class EventSignalCombinerTest( unittest.TestCase ) :

	def trueSlot( self, gadget, event ) :

		self.trueSlotCalled = True
		return True

	def falseSlot( self, gadget, event ) :

		self.falseSlotCalled = True
		return False
		
	def setUp( self ) :
	
		self.falseSlotCalled = False
		self.trueSlotCalled = False
			
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
	
if __name__ == "__main__":
	unittest.main()
	
