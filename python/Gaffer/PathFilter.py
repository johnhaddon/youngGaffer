import weakref
import new

import IECore

from Path import Path

## PathFilters are classes which can filter the results
# of Path.children() methods to provide a masked view of
# filesystems. Filters are applied to a path using the Path.addFilter()
# method.
class PathFilter( object ) :

	def __init__( self ) :
	
		pass
	
	## Must be implemented by derived classes to filter the passed
	# list of paths and return a new list.
	def filter( self, paths ) :
	
		raise NotImplementedError	
