import unittest

import IECore

import Gaffer
import GafferUI

class EventLoopTest( unittest.TestCase ) :

	def testIdleCallbacks( self ) :

		self.__idleCalls = 0
			
		def idle() :
		
			self.__idleCalls += 1
			return self.__idleCalls < 2
		
		def stop() :
			
			if self.__idleCalls==2 :
				GafferUI.EventLoop.stop()
				
			return True
			
		GafferUI.EventLoop.addIdleCallback( idle )
		stopID = GafferUI.EventLoop.addIdleCallback( stop )
		GafferUI.EventLoop.start()
	
		self.assertEqual( self.__idleCalls, 2 )
		
		GafferUI.EventLoop.removeIdleCallback( stopID )
		
if __name__ == "__main__":
	unittest.main()
	
