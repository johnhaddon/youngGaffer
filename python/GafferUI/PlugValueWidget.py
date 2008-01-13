from Widget import Widget

class PlugValueWidget( Widget ) :

	def __init__( self, gtkWidget, plug ) :
	
		Widget.__init__( self, gtkWidget )
	
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
			
	@classmethod
	def create( cls, plug ) :

		typeId = plug.typeId()
		if typeId in cls.__typesToCreators :
		
			return cls.__typesToCreators[typeId]( plug )
			
		return None
	
	@classmethod
	def registerType( cls, typeId, creator ) :
	
		cls.__typesToCreators[typeId] = creator
	
	__typesToCreators = {}	
		
	def __plugSetSlot( self, plug ) :
	
		if plug.isSame( self.__plug ) :
		
			self.updateFromPlug()	
