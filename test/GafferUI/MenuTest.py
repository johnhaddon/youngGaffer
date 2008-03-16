import IECore
import Gaffer
import GafferUI
import IECore
import unittest

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
	
