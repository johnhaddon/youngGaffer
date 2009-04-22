import IECore
import Gaffer

class test( Gaffer.Application ) :

	def __init__( self ) :
	
		Gaffer.Application.__init__( self )
		
		self.parameters().addParameters(
		
			[
				IECore.StringParameter(
					name = "testCase",
					description = "The name of a specific test case to run. If unspecified then all test cases are run.",
					defaultValue = "",
				)
			]
		
		)
				
	def doRun( self, args ) :
	
		import sys
		import unittest
		import GafferTest
		import GafferUITest
		
		testSuite = unittest.TestSuite()
		if args["testCase"].value :
		
			testCase = unittest.defaultTestLoader.loadTestsFromName( args["testCase"].value )
			testSuite.addTest( testCase )
			
		else :
		
			for module in ( GafferTest, GafferUITest ) :
		
				moduleTestSuite = unittest.defaultTestLoader.loadTestsFromModule( module )
				testSuite.addTest( moduleTestSuite )
			

		testRunner = unittest.TextTestRunner( verbosity=2 )
		testResult = testRunner.run( testSuite )
		
		return 0 if testResult.wasSuccessful() else 1
