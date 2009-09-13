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
		self.__eventBox.connect( "size-allocate", self.__allocate )
		
		# when we aren't split we might have a child
		self.__child = None
		
		# when we are split we have a pane and two Panels
		self.__paned = None
		self.__subPanels = None
		self.__positionToSetOnAllocate = None
				
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
		
	def split( self, direction, childSubPanelIndex=0, position=0.5 ) :

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
		self.__paned.pack1( self.__subPanels[0].gtkWidget(), True, True )
		self.__paned.pack2( self.__subPanels[1].gtkWidget(), True, True )
			
		if child :
			self.__subPanels[childSubPanelIndex].setChild( child )
		
		self.__paned.show()
		self.__eventBox.add( self.__paned )
				
		self.setSplitPosition( position )
		
	## Position runs from 0-1, left to right and bottom to top.
	def setSplitPosition( self, position ) :
	
		assert( self.isSplit() )
		
		if self.splitDirection()==self.SplitDirection.Vertical :
			size = self.__eventBox.allocation.width
		else :
			size = self.__eventBox.allocation.height
			position = 1 - position
		
		if size	> 1 :
			self.__paned.set_position( int( size * position ) )
		else :
			# trying to set the position when we have no allocation
			# is a bad idea. we always end up with a split line stuck
			# at the edge. so we store the position and set it later
			# in __allocate when we have sufficient size. 
			self.__positionToSetOnAllocate = position
	
	def getSplitPosition( self ) :
	
		if self.__positionToSetOnAllocate is not None :
			return self.__positionToSetOnAllocate
		
		position = self.__paned.get_position()
			
		if self.splitDirection()==self.SplitDirection.Vertical :
			size = self.__eventBox.allocation.width
		else :
			size = self.__eventBox.allocation.height
			position = size - position
		
		return float( position ) / float( size )
		
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

	def __allocate( self, gtkWidget, allocation ) :
			
		if self.__positionToSetOnAllocate is not None :
					
			if self.splitDirection()==self.SplitDirection.Vertical :
				size = allocation.width
			else :
				size = allocation.height
		
			self.__paned.set_position( int( size * self.__positionToSetOnAllocate ) )
			self.__positionToSetOnAllocate = None

Widget._parseRCStyle(

	"""
	style "gafferPaned" = "gafferWidget"
	{
		GtkPaned::handle-size = 3
	}

	widget_class "*<GtkPaned>" style "gafferPaned"
	""",

)
