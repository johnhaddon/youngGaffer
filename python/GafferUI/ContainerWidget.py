from Widget import Widget

## The ContainerWidget class provides a base for all
# Widgets which may hold other Widgets as children.
class ContainerWidget( Widget ) :

	def __init__( self, gtkWidget ) :
	
		GafferUI.Widget.__init__( self, gtkWidget )
	
	## Must be implemented in subclasses to remove
	# any references to the specified child. This allows
	# reparenting of that child into another ContainerWidget.
	def removeChild( self, child ) :
	
		raise NotImplementedError
