import unittest
from GafferUI.CamelCase import *

class CamelCaseTest( unittest.TestCase ) :

	def testSplit( self ) :
	
		self.assertEqual( split( "A" ), [ "A" ] )
		self.assertEqual( split( "a" ), [ "a" ] )
		self.assertEqual( split( "AB" ), [ "AB" ] )
		self.assertEqual( split( "ab" ), [ "ab" ] )
		self.assertEqual( split( "aB" ), [ "a", "B" ] )
		self.assertEqual( split( "Ab" ), [ "Ab" ] )
		self.assertEqual( split( "TIFFImageReader" ), [ "TIFF", "Image", "Reader" ] )
		self.assertEqual( split( "camelCase" ), [ "camel", "Case" ] )
		self.assertEqual( split( "hsvToRGB" ), [ "hsv", "To", "RGB" ] )
		
	def testJoin( self ) :
	
		self.assertEqual( join( [ "camel", "case" ], Caps.Unchanged ), "camelcase" )
		self.assertEqual( join( [ "camel", "case" ], Caps.First ), "Camelcase" )
		self.assertEqual( join( [ "camel", "case" ], Caps.All ), "CamelCase" )
		self.assertEqual( join( [ "camel", "case" ], Caps.AllExceptFirst ), "camelCase" )

		self.assertEqual( join( [ "TIFF", "image", "reader" ], Caps.Unchanged ), "TIFFimagereader" )
		self.assertEqual( join( [ "TIFF", "image", "reader" ], Caps.First ), "TIFFimagereader" )
		self.assertEqual( join( [ "TIFF", "image", "reader" ], Caps.All ), "TIFFImageReader" )
		self.assertEqual( join( [ "TIFF", "image", "reader" ], Caps.AllExceptFirst ), "tiffImageReader" )
		
	def testToSpaced( self ) :
	
		self.assertEqual( toSpaced( "camelCase" ), "Camel Case" )
		self.assertEqual( toSpaced( "camelCase", Caps.All ), "Camel Case" )
		self.assertEqual( toSpaced( "camelCase", Caps.First ), "Camel case" )
		self.assertEqual( toSpaced( "camelCase", Caps.AllExceptFirst ), "camel Case" )

		self.assertEqual( toSpaced( "TIFFImageReader" ), "TIFF Image Reader" )
		self.assertEqual( toSpaced( "TIFFImageReader", Caps.All ), "TIFF Image Reader" )
		self.assertEqual( toSpaced( "TIFFImageReader", Caps.First ), "TIFF image reader" )
		self.assertEqual( toSpaced( "TIFFImageReader", Caps.AllExceptFirst ), "tiff Image Reader" )
		
	def testFromSpaced( self ) :
	
		self.assertEqual( fromSpaced( "camel case" ), "CamelCase" )
		self.assertEqual( fromSpaced( "camel case", Caps.All ), "CamelCase" )
		self.assertEqual( fromSpaced( "camel case", Caps.First ), "Camelcase" )
		self.assertEqual( fromSpaced( "camel case", Caps.AllExceptFirst ), "camelCase" )
	
if __name__ == "__main__":
	unittest.main()
	
