import IECore
import gtk

from Menu import Menu
from Widget import Widget

## \todo Add joining to the menus and make sure it works
## \todo Implement an option to float in a new window, and an option to anchor back
## \todo Ctrl drag moves divider and other close by dividers, or the opposite way round
## \todo Figure out the size allocation to work better - moving one slider doesn't move the others
## \todo Click and drag based interactive splitting rather than menu based?
## \todo Serialisation using __repr__ (requires serialisation of children too)
class Panel( Widget ) :

	SplitDirection = IECore.Enum.create( "None", "Vertical", "Horizontal" )

	def __init__( self ) :
	
		Widget.__init__( self )
		
		self.__eventBox = gtk.EventBox()
		self.__eventBox.connect( "button-press-event", self.__buttonPress )
		self.setGTKWidget( self.__eventBox )
		self.__eventBox.show()
		
		self.__paned = None
		
	def setChild( self, child, index=0 ) :
				
		if self.isSplit() :
		
			if index < 0 or index > 1 :
				raise IndexError()
		
			if index==0 :
				pack = self.__paned.pack1
				get = self.__paned.get_child1
			else :
				pack = self.__paned.pack2
				get = self.__paned.get_child2
		
			if child :
				pack( child.getGTKWidget(), True, True )
			else :
				oldChild = get()
				if oldChild :
					self.__paned.remove( oldChild )
						
		else :
		
			if index != 0 :
				raise IndexError()
			
			oldChild = self.__eventBox.get_child()
			if oldChild :
				self.__eventBox.remove( oldChild )
				
			if child :
				child.getGTKWidget().show_all()
				self.__eventBox.add( child.getGTKWidget() )
			
		assert( child is self.getChild( index ) )

	def getChild( self, index=0 ) :
	
		if self.isSplit() :
		
			if index < 0 or index > 1 :
				raise IndexError()
			
			if index==0 :
				return Widget.owner( self.__paned.get_child1() )
			else :
				return Widget.owner( self.__paned.get_child2() )
				
		else :
		
			if index != 0 :
				raise IndexError()
				
			return Widget.owner( self.__eventBox.get_child() )
		
	def isSplit( self ) :
	
		return not self.__paned is None
		
	def split( self, direction, childIndex=0 ) :

		if self.isSplit() :
			raise Exception( "Cannot call split() when isSplit() is True." )
		
		if not isinstance( direction, Panel.SplitDirection ) or not direction :
			raise TypeError( "Split direction not valid" )
			
		child = self.getChild( 0 )

		if direction==self.SplitDirection.Vertical :
			self.__paned = gtk.HPaned()
		else :
			self.__paned = gtk.VPaned()
			
		if child :
			self.__eventBox.remove( child.getGTKWidget() )
		
		self.setChild( Panel(), 0 )
		self.setChild( Panel(), 1 )
		
		if child :
			self.getChild( childIndex ).setChild( child )
				
		self.__paned.show_all()
		self.__eventBox.add( self.__paned )
	
	def splitDirection( self ) :
	
		if not self.isSplit() :
			return self.SplitDirection.None
			
		if isinstance( self.__paned, gtk.HPaned ) :
			return self.SplitDirection.Vertical
			
		return self.SplitDirection.Horizontal

	def join( self, childIndexToKeep=0 ) :
	
		raise NotImplementedError
		
	def menuDefinition( self ) :
	
		m = IECore.MenuDefinition()
						
		for l, c in self.__contentCreators.items() :
		
			m.append( "/" + l, { "command" : IECore.curry( self.__cc, c, 0 ) } )
		
		m.append( "/divider", { "divider" : True } )
	
		m.append( "/splitLeft", { "command" : IECore.curry( self.split, self.SplitDirection.Vertical, 1 ) } )
		m.append( "/splitRight", { "command" : IECore.curry( self.split, self.SplitDirection.Vertical, 0 ) } )
		m.append( "/splitBottom", { "command" : IECore.curry( self.split, self.SplitDirection.Horizontal, 0 ) } )
		m.append( "/splitTop", { "command" : IECore.curry( self.split, self.SplitDirection.Horizontal, 1 ) } )
	
		return m
	
	def __cc( self, childCreator, childIndex ) :
	
		self.setChild( childCreator(), childIndex )
	
	def __buttonPress( self, widget, event ) :
	
		if event.button==3 :
		
			# right click
			m = Menu( self.menuDefinition() )
			m.popup()
			
			return True
		
		return False

	__contentCreators = {}
	
	@classmethod
	def registerContentCreator( cls, label, creator ) :
	
		cls.__contentCreators[label] = creator
		
Panel.registerContentCreator( "Empty", lambda : None )	
