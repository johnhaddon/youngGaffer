import pygtk
pygtk.require( "2.0" )

from Menu import Menu


## \todo Move the settings munging somewhere else
# - perhaps this is best done by the Preferences node?
import gtk
settings = gtk.settings_get_default()

m = gtk.Menu() # have to make a menu for the property to be created
del m
settings.set_property( "gtk-menu-popup-delay", 0 )
