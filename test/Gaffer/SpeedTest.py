import unittest

import IECore

import Gaffer

# original (r330):
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
# replace string comparisons with InternedString comparisons (r331):
#
#	5.161s
#	5.140s
#	5.138s
#
class SpeedTest( unittest.TestCase ) :

	def test( self ) :
	
		s = Gaffer.ScriptNode()
		
		for i in range( 0, 1000 ) :
			n = Gaffer.AddNode()
			s.addChild( n )	
				
if __name__ == "__main__":
	unittest.main()
	
