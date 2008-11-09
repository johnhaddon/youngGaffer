import weakref
import new

## Implements an object similar to weakref.proxy, except that
# it can work with bound methods.
class WeakMethod( object ) :

	def __init__( self, boundMethod ) :
	
		self.__class = boundMethod.im_class
		self.__method = boundMethod.im_func
		self.__self = weakref.ref( boundMethod.im_self )
		
	def __call__( self, *args, **kwArgs ) :
	
		m = new.instancemethod( self.__method, self.__self(), self.__class )
		return m( *args, **kwArgs )
