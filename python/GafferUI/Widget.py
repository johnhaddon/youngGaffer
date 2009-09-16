import weakref
import string

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
		
			if isinstance( gtkWidget, gtk.Menu ) :
				gtkWidget = gtkWidget.get_attach_widget()
			else :
				gtkWidget = gtkWidget.get_parent()
			
		return None
	
	__gtkWidgetOwners = weakref.WeakKeyDictionary()
			
	@staticmethod
	def _gtkColor( coreColor ) :
	
		c = coreColor.linearToSRGB()
		return gtk.gdk.Color( int( c[0] * 65535 ), int( c[1] * 65535 ), int( c[2] * 65535 ) )
		
	@staticmethod
	def _gtkRCColor( coreColor ) :
	
		c = coreColor.linearToSRGB()
		return "{ %f, %f, %f }" % tuple( c )
	
	@staticmethod
	def _parseRCStyle( template, substitutions={} ) :
	
		t = string.Template( template )
		s = t.safe_substitute( substitutions )
		gtk.rc_parse_string( s )
		gtk.rc_reset_styles( gtk.settings_get_default() )
	
	FontSize = IECore.Enum.create( "Small", "Medium", "Large" )
	FontWeight = IECore.Enum.create( "Light", "Normal", "Bold" )
	@staticmethod
	def _pangoFont( size=FontSize.Medium, weight=FontWeight.Normal ) :
	
		return string.Template(
			'font_family="sans" style="normal" weight="$weight" size="$size"'
		).substitute(
			{
				"weight" : {
					Widget.FontWeight.Light : "light",
					Widget.FontWeight.Normal : "normal",
					Widget.FontWeight.Bold : "bold",
				}[weight],
				"size" : {
					Widget.FontSize.Small : str( 9 * 1000 ),
					Widget.FontSize.Medium : str( 11 * 1000 ),
					Widget.FontSize.Large : str( 14 * 1000 ),
				}[size]
			}
		)
			
Widget._parseRCStyle(

	"""
	style "gafferWidget"
	{
		font_name = "Sans Bold 11"
		bg[NORMAL] = $bgNormal
		bg[ACTIVE] = $bgBlue
		bg[PRELIGHT] = $bgBlue
		bg[SELECTED] = $bgBlue
		bg[INSENSITIVE] = $bgInsensitive
		fg[NORMAL] = $fgNormal
		fg[ACTIVE] = $fgWhite
		fg[PRELIGHT] = $fgWhite
		fg[SELECTED] = $fgWhite
		fg[INSENSITIVE] = $fgInsensitive
		base[NORMAL] = $bgNormal
		base[ACTIVE] = $bgBlue
		base[PRELIGHT] = $bgBlue
		base[SELECTED] = $bgBlue
		base[INSENSITIVE] = $bgInsensitive
		text[NORMAL] = $fgNormal
		text[ACTIVE] = $fgWhite
		text[PRELIGHT] = $fgWhite
		text[SELECTED] = $fgWhite
		text[INSENSITIVE] = $fgInsensitive
	}

	style "gafferTextEntry" = "gafferWidget"
	{
		bg[NORMAL] = $textEntryBG
		base[NORMAL] = $textEntryBG
	}
	
	class "GtkWidget" style "gafferWidget"
	""",
	
	{
		"bgNormal" : Widget._gtkRCColor( IECore.Color3f( 0.07 ) ),
		"bgBlue" : Widget._gtkRCColor( IECore.Color3f( 0, 0.03, 0.2 ) ),
		"bgInsensitive" : Widget._gtkRCColor( IECore.Color3f( 0.05 ) ),
		"fgNormal" : Widget._gtkRCColor( IECore.Color3f( 0.8 ) ),
		"fgWhite" : Widget._gtkRCColor( IECore.Color3f( 1 ) ),
		"fgInsensitive" : Widget._gtkRCColor( IECore.Color3f( 0.3 ) ),
		"textEntryBG" : Widget._gtkRCColor( IECore.Color3f( 0.1 ) ),
	}

)
