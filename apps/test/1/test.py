import IECore
import Gaffer

class test( Gaffer.Application ) :

	def __init__( self ) :
	
		Gaffer.Application.__init__( self )
				
	def doRun( self, args ) :
	
		import sys
		import unittest
		import GafferTest
		import GafferUITest
		
		testSuite = unittest.TestSuite()
		for module in ( GafferTest, GafferUITest ) :
		
			moduleTestSuite = unittest.defaultTestLoader.loadTestsFromModule( module )
			testSuite.addTest( moduleTestSuite )
			

		testRunner = unittest.TextTestRunner( verbosity=2 )
		testResult = testRunner.run( testSuite )
		
		return 0 if testResult.wasSuccessful() else 1
