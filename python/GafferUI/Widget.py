import weakref

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

	def show( self ) :
	
		self.getGTKWidget().show()
		
	def hide( self ) :
	
		self.getGTKWidget().hide()

	## Returns the GafferUI.Widget which is the parent for this
	# Widget, or None if it has no parent.
	def parent( self ) :
	
		w = self.getGTKWidget()
		w = w.get_parent()
		while w :
		
			if w in Widget.__gtkWidgetOwners :
				return Widget.__gtkWidgetOwners[w]()
				
			w = w.get_parent()

		return None
		
	## Returns the top level gtk.Widget instance used to implement
	# the GafferUI.Widget functionality.
	def getGTKWidget( self ) :
	
		return self.__gtkWidget
	
	## Must be called by subclasses. Should be considered protected.	
	def setGTKWidget( self, w ) :
		
		if self.__gtkWidget :
			del Widget.__gtkWidgetOwners[self.__gtkWidget]
			
		self.__gtkWidget = w
		Widget.__gtkWidgetOwners[w] = weakref.ref( self )
	
	## Returns the GafferUI.Widget that owns the specified gtk.Widget
	@classmethod
	def owner( cls, gtkWidget ) :
	
		while gtkWidget :
	
			if gtkWidget in cls.__gtkWidgetOwners :
				return cls.__gtkWidgetOwners[gtkWidget]()
		
			gtkWidget = gtkWidget.get_parent()
			
		return None
		
	__gtkWidgetOwners = {}
