from _Gaffer import _UndoContext

class UndoContext() :

	State = _UndoContext.State

	def __init__( self, script, state=_UndoContext.State.Enabled ) :
	
		self.__script = script
		self.__state = state

	def __enter__( self ) :

		self.__context = _UndoContext( self.__script, self.__state )
		
	def __exit__( self, type, value, traceBack ) :

		del self.__context
