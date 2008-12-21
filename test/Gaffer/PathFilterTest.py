import unittest
import glob

import IECore

import Gaffer

class PathFilterTest( unittest.TestCase ) :

	def test( self ) :
	
		path = Gaffer.FileSystemPath( "test/data/scripts" )
		children = path.children()
		self.assertEqual( len( children ), len( glob.glob( "test/data/scripts/*" ) ) )
		
		# attach a filter
		gfrFilter = Gaffer.FileNamePathFilter( [ "*.gfr" ] )
		path.addFilter( gfrFilter )
		
		children = path.children()
		self.assertEqual( len( children ), len( glob.glob( "test/data/scripts/*.gfr" ) ) )
		
		# copy the path and check the filter is working on the copy
		pathCopy = path.copy()
		self.assertEqual( len( pathCopy.children() ), len( children ) )
		
		# detach the filter and check that behaviour has reverted
		path.removeFilter( gfrFilter )
		children = path.children()
		self.assertEqual( len( children ), len( glob.glob( "test/data/scripts/*" ) ) )
			

		
if __name__ == "__main__":
	unittest.main()
	
