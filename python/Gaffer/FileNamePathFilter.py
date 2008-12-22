import fnmatch
import re

from PathFilter import PathFilter

## A PathFilter which filters based on filename.
class FileNamePathFilter( PathFilter ) :

	## Matchers is a list of compiled regular expressions and/or
	# shell style pattern strings. The latter will be compiled 
	# into regular expressions using fnmatch.translate().
	# The resulting filter will pass through any path whose
	# name matches one or more of the regular expressions. If filterLeafOnly
	# is True then directories will always be passed through.
	def __init__( self, matchers, leafOnly=True ) :
	
		assert( isinstance( matchers, ( list, tuple ) ) )
	
		self.__matchers = []
		for m in matchers :
		
			if isinstance( m, basestring ) :
				self.__matchers.append( re.compile( fnmatch.translate( m ) ) )
			else :
				assert( type( m ) is type( re.compile( "" ) ) )
				self.__matchers.append( m )

		self.__leafOnly = leafOnly

	def filter( self, paths ) :
		
		result = []
		for p in paths :
			if len( p ) :
				if self.__leafOnly and not p.isLeaf() :
					result.append( p )
				else :
					for m in self.__matchers :
						if m.match( p[-1] ) :
							result.append( p )
							break

		return result
