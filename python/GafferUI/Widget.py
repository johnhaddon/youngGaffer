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

	## Derived classes must create an appropriate gtk widget to be the top
	# level for their implementation and pass it to the Widget.__init__ method.
	# The widget can subsequently be accessed using gtkWidget() but it cannot be
	# replaced with another.
	def __init__( self, gtkWidget ) :
	
		self.__gtkWidget = gtkWidget
		Widget.__gtkWidgetOwners[gtkWidget] = weakref.ref( self )

	## \todo Should be setVisible() and getVisible() for consistency
	# \todo Define whether a widget is shown or not by default and make sure
	# they all behave that way - right now i'm preferring shown by default.
	def show( self ) :
	
		self.gtkWidget().show()
		
	def hide( self ) :
	
		self.gtkWidget().hide()

	## Returns the GafferUI.Widget which is the parent for this
	# Widget, or None if it has no parent.
	def parent( self ) :
	
		w = self.gtkWidget()
		w = w.get_parent()
		while w :
		
			if w in Widget.__gtkWidgetOwners :
				return Widget.__gtkWidgetOwners[w]()
				
			w = w.get_parent()

		return None
		
	## Returns the top level gtk.Widget instance used to implement
	# the GafferUI.Widget functionality.
	def gtkWidget( self ) :
	
		return self.__gtkWidget
		
	## Returns the GafferUI.Widget that owns the specified gtk.Widget
	@classmethod
	def owner( cls, gtkWidget ) :
	
		while gtkWidget :
	
			if gtkWidget in cls.__gtkWidgetOwners :
				return cls.__gtkWidgetOwners[gtkWidget]()
		
			gtkWidget = gtkWidget.get_parent()
			
		return None
		
	__gtkWidgetOwners = weakref.WeakKeyDictionary()
