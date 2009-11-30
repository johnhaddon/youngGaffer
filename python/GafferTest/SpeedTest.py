import unittest

import IECore

import Gaffer
import GafferTest

class SpeedTest( unittest.TestCase ) :

	# original (r335):
	#
	#	15.110s
	#	15.088s
	#	15.102s
	#
	# interned string :
	#
	#	17.288s
	#	17.216s
	#	17.213s
	#
	# no setName in addChildInternal (still interned string) :
	#
	#	0.104s
	#	0.099s
	#	0.099s
	#
	# no setName in addChildInternal (std::string) :
	#
	#	0.103s
	#	0.098s
	#	0.098s
	#
	# replace string comparisons with InternedString comparisons (r336):
	#
	#	5.161s
	#	5.140s
	#	5.138s
	#
	def testMakeNamesUnique( self ) :
	
		s = Gaffer.ScriptNode()
		
		for i in range( 0, 1000 ) :
			n = GafferTest.AddNode()
			s.addChild( n )	
	
	#
	# this test checks it doesn't take a ludicrous amount of time
	# to retrieve children from their parents by name. even though
	# we're currently doing a linear search to achieve this it doesn't
	# seem to be a particularly pressing issue, perhaps because comparison
	# against many InternedStrings is much cheaper than comparison
	# against many std::strings. if necessary we can improve things by
	# storing a map from name to children in GraphComponent.
	#
	# r338 (linear search with string comparisons)
	#
	#	0.214s
	#	0.183s
	#	0.172s
	#
	# r339 (linear search with InternedString comparisons)
	#
	#	0.146s
	#	0.136s
	#	0.140s
	def testGetChild( self ) :
	
		s = Gaffer.ScriptNode()
		
		for i in range( 0, 1000 ) :
			# explicitly setting the name to something unique
			# avoids the overhead incurred by the example
			# in testMakeNamesUnique
			n = GafferTest.AddNode( "AddNode" + str( i ) )
			s.addChild( n )	
		
		for i in range( 0, 1000 ) :
			n = "AddNode" + str( i )
			c = s[n]
			self.assertEqual( c.getName(), n )
					
if __name__ == "__main__":
	unittest.main()
	
