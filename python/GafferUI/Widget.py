## The Widget class provides a base class for all widgets in GafferUI.
# GafferUI.Widget subclasses are implemented using gtk widgets, but the public API
# exposed by the GafferUI classes never includes gtk functions or classes directly - this
# allows the implementation to be changed in the future. Furthermore it allows the use
# of the GafferUI module without learning all the gtk API. To enforce this separation,
# GafferUI classes must not derive from gtk classes.
#
# \todo Consider how this relates to the other Widget class we'll be making for the GL editors
class Widget() :

	def __init__( self ) :
	
		self.__gtkWidget = None

	## Returns the parent gtk.Widget instance used to implement
	# the GafferUI.Widget functionality.
	def getGTKWidget( self ) :
	
		return self.__gtkWidget
	
	## Must be called by subclasses. Should be considered protected.	
	def setGTKWidget( self, w ) :
		
		self.__gtkWidget = w
