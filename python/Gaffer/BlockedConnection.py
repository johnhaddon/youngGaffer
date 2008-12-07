class BlockedConnection() :

	def __init__( self, connection ) :
	
		self.__connection = connection

	def __enter__( self ) :
	
		assert( not self.__connection.blocked() )
		self.__connection.block()
		
	def __exit__( self, type, value, traceBack ) :
	
		assert( self.__connection.blocked() )
		self.__connection.unblock()
		
