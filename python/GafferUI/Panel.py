import IECore
import gtk

from Menu import Menu

## \todo Add splitting/joining to the menus and make sure they work
## \todo Implement an option to float in a new window
## \todo Figure out how to get the menu showing when the child wants their own
# goddam menu/event handler
class Panel() :

	SplitDirection = IECore.Enum.create( "None", "Vertical", "Horizontal" )

	def __init__( self ) :
	
		self.gtkWidget = gtk.EventBox()
		self.gtkWidget.connect( "button-press-event", self.__buttonPress )
		self.__paned = None
		
	def setChild( self, child, index=0 ) :
		
		if self.isSplit() :
		
			if index < 0 or index > 1 :
				raise IndexError()
		
			if index==0 :
				self.__paned.pack1( child.gtkWidget, True, True )
			else :
				self.__paned.pack2( child.gtkWidget, True, True )
						
		else :
		
			if index != 0 :
				raise IndexError()
			
			oldChild = self.gtkWidget.get_child()
			if oldChild :
				self.gtkWidget.remove( oldChild )
				
			child.gtkWidget.show_all()
			self.gtkWidget.add( child.gtkWidget )

	def getChild( self, index=0 ) :
	
		if self.isSplit() :
		
			if index < 0 or index > 1 :
				raise IndexError()
			
			if index==0 :
				return self.__paned.get_child1()
			else :
				return self.__paned.get_child2()
				
		else :
		
			if index != 0 :
				raise IndexError()
				
			return self.gtkWidget.get_child()
		
		
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
			
		self.gtkWidget.remove( child )
		self.gtkWidget.add( self.__paned )
		
		self.setChild( child, childIndex )
	
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
			
		return m
	
	def __cc( self, childCreator, childIndex ) :
	
		self.setChild( childCreator(), childIndex )
	
	def __buttonPress( self, widget, event ) :
	
		print "BUTTON PRESS"
		if event.button==3 :
		
			# right click
			
			m = Menu( self.menuDefinition() )
			m.gtkWidget.popup( None, None, None, event.button, event.time )

	__contentCreators = {}
	@classmethod
	def registerContentCreator( cls, label, creator ) :
	
		cls.__contentCreators[label] = creator

class EmptyThing() :

	def __init__( self ) :
		
		self.gtkWidget = gtk.Frame()
		self.gtkWidget.set_shadow_type( gtk.SHADOW_OUT )

Panel.registerContentCreator( "Empty", EmptyThing )	
