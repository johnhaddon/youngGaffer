import GafferUI
import unittest

class WindowTest( unittest.TestCase ) :

	def testTitle( self ) :
	
		w = GafferUI.Window()
		self.assertEqual( w.getTitle(), "GafferUI.Window" )
		
		w = GafferUI.Window( "myTitle" )
		self.assertEqual( w.getTitle(), "myTitle" )
		
		w.setTitle( "myOtherTitle" )
		self.assertEqual( w.getTitle(), "myOtherTitle" )
				
if __name__ == "__main__":
	unittest.main()
	
