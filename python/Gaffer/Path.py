import Gaffer

class Path( object ) :

	def __init__( self, path ) :
	
		self.__items = []
		
		if isinstance( path, basestring ) :
			
			self.setFromString( path )
			
		else :
		
			for p in path :
					
				self.__checkElement( p )		
				self.__items.append( p )
				
		self.__filters = set()
									
	## Returns true if this path is valid - ie references something
	# which actually exists.
	def isValid( self ) :
	
		raise NotImplementedError
	
	## If the path is valid, returns a dictionary of information about what
	# the path points to. If the path is not valid, returns None. The contents
	# of the dictionary depends on the type of Path. Subclasses should call
	# their base class' implementation first, and then add additional information
	# to the dictionary returned.
	def info( self ) :
	
		if not self.isValid() :
			return None
			
		result = {}
		
		return result
	
	## Returns true if this path can never have child Paths.
	def isLeaf( self ) :
	
		raise NotImplementedError
	
	## Returns the parent of this path, or None if the path
	# has no parent (is the root).
	def parent( self ) :
	
		if not self.__items :
			return None
			
		parent = self.copy()
		del parent[-1]
		
		return parent
	
	## Returns a list of Path instances representing all
	# the children of this path. Note that an empty list may
	# be returned even if isLeaf() is False.
	def children( self ) :
	
		c = self._children()
			
		for f in self.__filters :
			c = f.filter( c )
			
		return c	
	
	## The subclass specific part of children(). This must be implemented
	# by subclasses to return a list of children - filtering will be applied
	# in the children() method so can be ignored by the derived classes.	
	def _children( self ) :
	
		raise NotImplementedError
		
	def addFilter( self, pathFilter ) :
	
		self.__filters.add( pathFilter )
		self.__emitChangedSignal()
		
	def removeFilter( self, pathFilter ) :
	
		self.__filters.remove( pathFilter )
		self.__emitChangedSignal()

	def pathChangedSignal( self ) :
	
		if not hasattr( self, "_Path__pathChangedSignal" ) :
			self.__pathChangedSignal = Gaffer.ObjectSignal()
			
		return self.__pathChangedSignal

	def setFromString( self, path ) :
	
		if len( path ) and path[0]!="/" :
			raise ValueError( "Paths must be absolute (start with \"/\")." )
		
		prev = self.__items
			
		self.__items = path.split( "/" )
		self.__items = [ x for x in self.__items if x ]

		if prev!=self.__items :
			self.__emitChangedSignal()
	
	def copy( self ) :
	
		c = self.__class__( self.__items )
		c.__filters = set( self.__filters )
		
		return c
		
	def append( self, element ) :
	
		self.__checkElement( element )
		self.__items.append( element )
		self.__emitChangedSignal()
	
	def truncateUntilValid( self ) :
	
		changed = False
		while len( self.__items ) and not self.isValid() :
			del self.__items[-1]
			changed = True
			
		if changed :
			self.__emitChangedSignal()
			
	def __len__( self ) :
	
		return len( self.__items )

	def __str__( self ) :
		
		return "/" + "/".join( self.__items )
			
	def __setitem__( self, index, name ) :
	
		if isinstance( index, slice ) :
			for n in name :
				self.__checkElement( n )
		else :
			self.__checkElement( name )
		
		prev = self.__items[index]
				
		self.__items.__setitem__( index, name )
		
		if prev!=name :
			self.__emitChangedSignal()
		
	def __getitem__( self, index ) :
	
		return self.__items.__getitem__( index )

	def __delitem__( self, index ) :
	
		self.__items.__delitem__( index )
		self.__emitChangedSignal()
		
	def __eq__( self, other ) :
	
		if not isinstance( other, Path ) :
			return False
			
		return self.__items == other.__items
		
	def __ne__( self, other ) :
	
		if not isinstance( other, Path ) :
			return True
			
		return self.__items != other.__items	
			
	def __checkElement( self, element ) :
	
		if not isinstance( element, basestring ) :
			raise ValueError( "Path elements must be strings." )
				
		if "/" in element :
			raise ValueError( "Path element contains \"/\"." )
		
		if element=="" :
			raise ValueError( "Path element is empty." )	

	def __emitChangedSignal( self ) :
		
		if hasattr( self, "_Path__pathChangedSignal" ) :
			self.__pathChangedSignal( self )
