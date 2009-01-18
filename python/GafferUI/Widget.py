import weakref

import gtk
import IECore

## The Widget class provides a base class for all widgets in GafferUI.
# GafferUI.Widget subclasses are implemented using gtk widgets, but the public API
# exposed by the GafferUI classes never includes gtk functions or classes directly - this
# allows the implementation to be changed in the future. Furthermore it allows the use
# of the GafferUI module without learning all the gtk API. To enforce this separation,
# GafferUI classes must not derive from gtk classes.
#
# \todo Consider how this relates to the other Widget class we'll be making for the GL editors
class Widget( object ) :

	## Derived classes must create an appropriate gtk widget to be the top
	# level for their implementation and pass it to the Widget.__init__ method.
	# The widget can subsequently be accessed using gtkWidget() but it cannot be
	# replaced with another.
	def __init__( self, gtkWidget ) :
	
		self.__gtkWidget = gtkWidget
		Widget.__gtkWidgetOwners[gtkWidget] = weakref.ref( self )
		
		self._setDefaultColors( self.__gtkWidget, False )
		self.show()
		
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
				
		while w :
					
			if isinstance( w, gtk.Menu ) :
				w = w.get_attach_widget()	
			else :
				w = w.get_parent()
		
			if w in Widget.__gtkWidgetOwners :
				return Widget.__gtkWidgetOwners[w]()
				
		return None
	
	## Returns the first Widget in the hierarchy above this one
	# to match the desired type.
	def ancestor( self, type ) :
	
		w = self
		while w :
			w = w.parent()
			if isinstance( w, type ) :
				return w
				
		return None
		
	## Returns the top level gtk.Widget instance used to implement
	# the GafferUI.Widget functionality.
	# \todo "Protect" with an _
	def gtkWidget( self ) :
	
		return self.__gtkWidget
		
	## Returns the GafferUI.Widget that owns the specified gtk.Widget
	# \todo "Protect" with an _
	@classmethod
	def owner( cls, gtkWidget ) :
	
		while gtkWidget :
	
			if gtkWidget in cls.__gtkWidgetOwners :
				return cls.__gtkWidgetOwners[gtkWidget]()
		
			gtkWidget = gtkWidget.get_parent()
			
		return None
	
	__gtkWidgetOwners = weakref.WeakKeyDictionary()
	
	State = IECore.Enum.create( "Normal", "Inactive", "Selected" )		
		
	@staticmethod
	def _gtkColor( coreColor ) :
	
		c = coreColor.linearToSRGB()
		return gtk.gdk.Color( int( c[0] * 65535 ), int( c[1] * 65535 ), int( c[2] * 65535 ) )	
	
	@staticmethod
	def _setColors( gtkWidget, state, fg, bg, recurse=False ) :
	
		gtkFG = Widget._gtkColor( fg )
		gtkBG = Widget._gtkColor( bg )
		
		if state==Widget.State.Normal :
			gtkStates = ( gtk.STATE_NORMAL, )
		elif state==Widget.State.Inactive :
			gtkStates = ( gtk.STATE_INSENSITIVE, )
		else :
			gtkStates = ( gtk.STATE_ACTIVE, gtk.STATE_PRELIGHT, gtk.STATE_SELECTED )
	
		for s in gtkStates :
				
			gtkWidget.modify_bg( s, gtkBG )
			gtkWidget.modify_base( s, gtkBG )

			gtkWidget.modify_fg( s, gtkFG )
			gtkWidget.modify_text( s, gtkFG )

		if recurse and isinstance( gtkWidget, gtk.Container ) :
			for c in gtkWidget.get_children() :				
				Widget._setColors( c, state, fg, bg, recurse )
		
	_defaultFGColors = [
		IECore.Color3f( 0.8 ),
		IECore.Color3f( 0.7 ),
		IECore.Color3f( 0.9 ),
	]
	
	_defaultBGColors = [
		IECore.Color3f( 0.07 ),
		IECore.Color3f( 0.05 ),
		IECore.Color3f( 0, 0.03, 0.2 ),
	]
	
	_textEntryBGColor = IECore.Color3f( 0.1 )
	_textEntryFGColor = IECore.Color3f( 1 )

	@staticmethod
	def _setDefaultColors( gtkWidget, recurse=False ) :
	
		Widget._setColors( gtkWidget, Widget.State.Normal, Widget._defaultFGColors[0], Widget._defaultBGColors[0], recurse )
		Widget._setColors( gtkWidget, Widget.State.Inactive, Widget._defaultFGColors[1], Widget._defaultBGColors[1], recurse )
		Widget._setColors( gtkWidget, Widget.State.Selected, Widget._defaultFGColors[2], Widget._defaultBGColors[2], recurse )
