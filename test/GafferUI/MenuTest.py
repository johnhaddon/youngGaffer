import GafferUI
import IECore
import unittest

## \todo Rejig this to remove all gtk specific code, and use only GafferUI classes instead

class MenuTest( unittest.TestCase ) :

	def test( self ) :
	
		definition = IECore.MenuDefinition(

			[
				( "/apple/pear/banana", { } ),
				( "/apple/pear/divider", { "divider" : True } ),
				( "/apple/pear/submarine", { } ),
				( "/dog/inactive", { "active" : False } ),
			]

		)

		menu = GafferUI.Menu( definition )

if __name__ == "__main__":
	unittest.main()
	
