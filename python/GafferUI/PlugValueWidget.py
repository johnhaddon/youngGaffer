class PlugValueWidget() :

	def __init__( self, plug ) :
	
		self.setPlug( plug )
		
	def setPlug( self, plug ) :
	
		self.__plug = plug
		self.__plugSetConnection = plug.node().plugSetSignal().connect( self.__plugSetSlot )
		self.updateFromPlug()
		
	def getPlug( self ) :
	
		return self.__plug
	
	## Must be implemented by subclasses so that the widget reflects the current
	# value of the plug.	
	def updateFromPlug( self ) :
	
		raise NotImplementedError
		
	def __plugSetSlot( self, plug ) :
	
		if plug.isSame( self.__plug ) :
		
			self.updateFromPlug()	
