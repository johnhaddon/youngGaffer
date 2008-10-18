import IECore
import gtk

from ContainerWidget import ContainerWidget
from Widget import Widget

## \todo Ctrl drag moves divider and other close by dividers, or the opposite way round
## \todo Figure out the size allocation to work better - moving one slider doesn't move the others
class Splittable( ContainerWidget ) :

	SplitDirection = IECore.Enum.create( "None", "Vertical", "Horizontal" )

	def __init__( self ) :
	
		ContainerWidget.__init__( self, gtk.EventBox() )
		
		# an event box is always our top level gtk widget
		self.__eventBox = self.gtkWidget()
		self.__eventBox.show()
		
		# when we aren't split we might have a child
		self.__child = None
		
		# when we are split we have a pane and two Panels
		self.__paned = None
		self.__subPanels = None
		
	def setChild( self, child ) :
		
		assert( not self.isSplit() )
		
		oldChild = self.getChild()
		if oldChild :
			self.removeChild( oldChild )
		
		if child :
		
			oldParent = child.parent()
			if oldParent :
				oldParent.removeChild( child )
				
			self.__child = child				
			self.__eventBox.add( self.__child.gtkWidget() )
			
		assert( child is self.getChild() )

	def getChild( self ) :
	
		assert( not self.isSplit() )
	
		if self.__child :
			assert( Widget.owner( self.__eventBox.get_child() ) is self.__child )
		
		return self.__child
		
	def removeChild( self, child ) :
	
		assert( not self.isSplit() )
		
		assert( child is self.__child )
		
		self.__eventBox.remove( self.__child.gtkWidget() )
		self.__child = None

	def isSplit( self ) :
	
		return not self.__paned is None
		
	def split( self, direction, childSubPanelIndex=0 ) :

		assert( not self.isSplit() )
		
		if not isinstance( direction, Splittable.SplitDirection ) or not direction :
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
		
		self.__subPanels = [ Splittable(), Splittable() ]
		self.__paned.pack1( self.__subPanels[0].gtkWidget() )
		self.__paned.pack2( self.__subPanels[1].gtkWidget() )
			
		if child :
			self.__subPanels[childSubPanelIndex].setChild( child )
		
		self.__paned.show()
		self.__eventBox.add( self.__paned )
		
		if direction==self.SplitDirection.Vertical :
			self.__paned.set_position( self.__eventBox.allocation.width / 2 )					
		else :
			self.__paned.set_position( self.__eventBox.allocation.height / 2 )					
		
		
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

	def subPanelIndex( self, panel ) :
	
		if self.subPanel( 0 ) is panel :
			return 0
		elif self.subPanel( 1 ) is panel :
			return 1
		else :
			return -1

	## Note that isSplit() may still be True following this call. This
	# occurs in the case where the sub panel being kept is itself split.
	def join( self, childToKeepPanelIndex=0 ) :
	
		assert( self.isSplit() )

		subPanelToKeep = self.subPanel( childToKeepPanelIndex )

		c = subPanelToKeep.__eventBox.get_child()
		if c :
			subPanelToKeep.__eventBox.remove( c )
							
		self.__eventBox.remove( self.__paned )
		self.__subPanels = subPanelToKeep.__subPanels
		self.__paned = subPanelToKeep.__paned
		self.__child = subPanelToKeep.__child
		
		if c :
			self.__eventBox.add( c )
