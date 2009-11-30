import unittest
import urllib2
import os

import IECore

import Gaffer

class AboutTest( unittest.TestCase ) :

	def test( self ) :
	
		for d in Gaffer.About.dependencies() :
		
			if "license" in d :
				f = os.path.expandvars( d["license"] )
				self.failUnless( os.path.exists( f ) )
			
			if "source" in d :
				self.assert_( urllib2.urlopen( d["source"] ) )
			
		
if __name__ == "__main__":
	unittest.main()
	
