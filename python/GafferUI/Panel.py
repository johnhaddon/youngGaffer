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
## 
#!!!!!!!!!! THIS IS FUCKED!!!!!!!!!!!!
#!!!!!!!!!! I THINK WE NEED TO CONSIDER OWNERSHIP OF THE GAFFERUI AND GTK WIDGETS MORE THOROUGHLY
#!!!!!!!!!! AND MAYBE HAVE AN ADD() AND REMOVE() METHOD IN THE BASE CLASS?
#!!!!!!!!!! TRY THIS :
#!!!!!!!!!! MAKE SCRIPT EDITOR
#!!!!!!!!!! SPLIT LEFT
#!!!!!!!!!! IN LEFT PANEL CHOOSE REMOVE - THE SCRIPT EDITOR DISAPPEARS
class Panel( Widget ) :

	SplitDirection = IECore.Enum.create( "None", "Vertical", "Horizontal" )

	def __init__( self ) :
	
		Widget.__init__( self )
		
		# an event box is always our top level gtk widget
		self.__eventBox = gtk.EventBox()
		self.__eventBox.connect( "button-press-event", self.__buttonPress )
		self.__eventBox.show()
		self.setGTKWidget( self.__eventBox )
		
		# when we aren't split we might have a child
		self.__child = None
		
		# when we are split we have a pane and two Panels
		self.__paned = None
		self.__subPanels = None
		
	def setChild( self, child ) :
		
		assert( not self.isSplit() )
		
		if self.__child :
			self.__eventBox.remove( self.__child.getGTKWidget() )
		
		self.__child = child				
		if self.__child :
			self.__eventBox.add( self.__child.getGTKWidget() )
			self.__eventBox.show_all()
			
		assert( child is self.getChild() )

	def getChild( self ) :
	
		assert( not self.isSplit() )
	
		if self.__child :
			assert( Widget.owner( self.__eventBox.get_child() ) is self.__child )
		
		return self.__child
	
	def isSplit( self ) :
	
		return not self.__paned is None
		
	def split( self, direction, childSubPanelIndex=0 ) :

		assert( not self.isSplit() )
		
		if not isinstance( direction, Panel.SplitDirection ) or not direction :
			raise TypeError( "Split direction not valid" )
			
		if childSubPanelIndex < 0 or childSubPanelIndex > 1 :
			raise IndexError( "Panel index out of range." )
			
		child = self.getChild()
		if child :
			self.setChild( None )

		if direction==self.SplitDirection.Vertical :
			self.__paned = gtk.HPaned()
		else :
			self.__paned = gtk.VPaned()
		
		self.__subPanels = [ Panel(), Panel() ]
		self.__paned.pack1( self.__subPanels[0].getGTKWidget() )
		self.__paned.pack2( self.__subPanels[1].getGTKWidget() )
			
		if child :
			self.__subPanels[childSubPanelIndex].setChild( child )
							
		self.__paned.show()
		self.__eventBox.add( self.__paned )
	
	def splitDirection( self ) :
	
		if not self.isSplit() :
			return self.SplitDirection.None
			
		if isinstance( self.__paned, gtk.HPaned ) :
			return self.SplitDirection.Vertical
			
		return self.SplitDirection.Horizontal

	def subPanel( self, index ) :
	
		assert( self.isSplit() )
		
		if index < 0 or index > 1 :
			raise IndexError( "Panel index out of range." )
		
		return self.__subPanels[index]

	def join( self, childToKeepPanelIndex=0 ) :
	
		assert( self.isSplit() )
		
		childToKeep = self.__subPanels[childToKeepPanelIndex].getChild()
		print "KEEPING", childToKeep
		
		self.__eventBox.remove( self.__paned )
		self.__paned = None
		self.__subPanels[0].setChild( None )
		self.__subPanels[1].setChild( None )
		self.__subPanels = None
		
		self.setChild( childToKeep )
		
		print "JOINED", self.getChild(), self.getChild().getGTKWidget(), self.__eventBox.get_child()
		self.getChild().getGTKWidget().show_all()
					
	def menuDefinition( self ) :
	
		## we can only really do useful things to leaf panels
		if self.isSplit() :
			return None
			
		m = IECore.MenuDefinition()
									
		for l, c in self.__contentCreators.items() :
			m.append( "/" + l, { "command" : IECore.curry( self.__setChildCallback, c ) } )

		m.append( "/divider", { "divider" : True } )

		m.append( "remove", { "command" : self.__removeCallback } )		
		
		m.append( "/divider2", { "divider" : True } )

		m.append( "/splitLeft", { "command" : IECore.curry( self.split, self.SplitDirection.Vertical, 1 ) } )
		m.append( "/splitRight", { "command" : IECore.curry( self.split, self.SplitDirection.Vertical, 0 ) } )
		m.append( "/splitBottom", { "command" : IECore.curry( self.split, self.SplitDirection.Horizontal, 0 ) } )
		m.append( "/splitTop", { "command" : IECore.curry( self.split, self.SplitDirection.Horizontal, 1 ) } )

		return m
	
	def __setChildCallback( self, childCreator ) :
	
		import weakref
		def cb( a ) :
			print "DYING!", a
			
		child = childCreator()
		self.__w = weakref.ref( child, cb )
		
		self.setChild( child )
		
	def __removeCallback( self ) :
	
		parent = self.parent()
		if isinstance( parent, Panel ) :
			print "PARENT IS PANEL"
			if self is parent.subPanel( 0 ) :
				print "CALLING JOIN 1"
				parent.join( 1 )
			else :
				print "CALLING JOIN 0"
				parent.join( 0 )
		else :
			self.setChild( None )
	
	def __buttonPress( self, widget, event ) :
	
		if event.button==3 :
			# right click
			m = self.menuDefinition()
			if m :
				m = Menu( m )
				m.popup()
				return True
		
		return False

	__contentCreators = {}
	
	@classmethod
	def registerContentCreator( cls, label, creator ) :
	
		cls.__contentCreators[label] = creator
		
Panel.registerContentCreator( "Empty", lambda : None )	
