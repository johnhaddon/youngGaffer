import weakref
import gc
import unittest

import IECore

class GCTest( unittest.TestCase ) :

	def test( self ) :
	
		"""Test that the garbage collection fix is present in Boost.Python."""

		BoostPythonClass = IECore.V3f

		# make a cyclic reference
		# between instances of a bound class
		o1 = BoostPythonClass()
		o2 = BoostPythonClass()
		o1.r = o2
		o2.r = o1

		# make weak references to track
		# their lifetime
		w1 = weakref.ref( o1 )
		w2 = weakref.ref( o2 )

		# delete the instances and
		# run the garbage collector
		del o1
		del o2
		while gc.collect() :
			pass

		# check that the objects have been collected
		self.assert_( w1() is None )
		self.assert_( w2() is None )

if __name__ == "__main__":
	unittest.main()
	
