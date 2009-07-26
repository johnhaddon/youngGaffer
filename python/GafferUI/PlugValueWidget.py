import Gaffer
from Widget import Widget

class PlugValueWidget( Widget ) :

	def __init__( self, gtkWidget, plug ) :
	
		Widget.__init__( self, gtkWidget )
	
		self.setPlug( plug )
		
	def setPlug( self, plug ) :
	
		self.__plug = plug

		if self.__plug is not None :
			self.__plugSetConnection = plug.node().plugSetSignal().connect( self.__plugSetSlot )
			self.__plugDirtiedConnection = plug.node().plugDirtiedSignal().connect( self.__plugDirtiedSlot )
			self.__plugInputChangedConnection = plug.node().plugInputChangedSignal().connect( self.__plugInputChangedSlot )
		else :
			self.__plugSetConnection = None
			self.__plugDirtiedConnection = None
			self.__plugInputChangedConnection = None

		self.updateFromPlug()
		
	def getPlug( self ) :
	
		return self.__plug
	
	## Must be implemented by subclasses so that the widget reflects the current
	# status of the plug.	
	def updateFromPlug( self ) :
	
		raise NotImplementedError
	
	## Returns True if the plug value is editable - that is the plug
	# is an input plug and it has no incoming connection.
	def _editable( self ) :
	
		plug = self.getPlug()
		
		if plug is None :
			return False
		
		if plug.direction()==Gaffer.Plug.Direction.Out :
			return False
		if plug.getInput() :
			return False
		
		return True
			
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

	def __plugDirtiedSlot( self, plug ) :
	
		if plug.isSame( self.__plug ) :
		
			self.updateFromPlug()	

	def __plugInputChangedSlot( self, plug ) :
	
		if plug.isSame( self.__plug ) :
		
			self.updateFromPlug()	
