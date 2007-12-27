import IECore
import Gaffer
import GafferUI
import IECore
import gtk

## \todo Rejig this to remove all gtk specific code, and use only GafferUI classes instead

window = gtk.Window( gtk.WINDOW_TOPLEVEL )
window.set_size_request( 200, 100 )
window.set_title( "Plug value widget test" )
window.connect( "delete_event", gtk.main_quit )
column = gtk.VBox()
window.add( column )

node = Gaffer.Node()
node.floatPlug = Gaffer.FloatPlug()
node.intPlug = Gaffer.IntPlug()

floatPlugWidget = GafferUI.NumericPlugValueWidget( node.floatPlug )
floatPlugWidget2 = GafferUI.NumericPlugValueWidget( node.floatPlug )
intPlugWidget = GafferUI.NumericPlugValueWidget( node.intPlug )
intPlugWidget2 = GafferUI.NumericPlugValueWidget( node.intPlug )

column.pack_start( floatPlugWidget.gtkWidget, False )
column.pack_start( floatPlugWidget2.gtkWidget, False )
column.pack_start( intPlugWidget.gtkWidget, False )
column.pack_start( intPlugWidget2.gtkWidget, False )


window.show_all()
gtk.main()
