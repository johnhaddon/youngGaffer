import os
import pwd
import grp

import Gaffer

class FileSystemPath( Gaffer.Path ) :

	def __init__( self, path ) :
	
		if isinstance( path, basestring ) :
			if len( path ) and path[0] != "/" :
				path = os.path.join( os.getcwd(), path )
	
		Gaffer.Path.__init__( self, path )
									
	## Returns true if this path is valid - ie references something
	# which actually exists.
	def isValid( self ) :
	
		return os.path.exists( str( self ) )
	
	## Returns true if this path can never have child Paths.
	def isLeaf( self ) :
	
		return not os.path.isdir( str( self ) )
	
	def info( self ) :
	
		result = Gaffer.Path.info( self )
		if result is None :
			return None
			
		s = os.stat( str( self ) )
		p = pwd.getpwuid( s.st_uid )
		g = grp.getgrgid( s.st_gid )
				
		result["fileSystem:owner"] = p.pw_name
		result["fileSystem:group"] = g.gr_name
		result["fileSystem:modificationTime"] = s.st_mtime
		result["fileSystem:accessTime"] = s.st_atime
		
		return result
	
	## Returns a list of Path instances representing all
	# this children of this path. Note that an empty list may
	# be returned even if isLeaf() is False.
	def children( self ) :
	
		try :
			c = os.listdir( str( self ) )
		except :
			return []
			
		return [ FileSystemPath( self[:] + [ x ] ) for x in c ]

