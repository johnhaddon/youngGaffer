import IECore
import Gaffer
import GafferUI
import IECore
import gtk

## \todo Rejig this to remove all gtk specific code, and use only GafferUI classes instead

window = gtk.Window( gtk.WINDOW_TOPLEVEL )
window.set_size_request( 200, 300 )
window.set_title( "Plug widget test" )
window.connect( "delete_event", gtk.main_quit )
column = gtk.VBox()
window.add( column )

node = Gaffer.Node()
node.floatPlug = Gaffer.FloatPlug()
node.intPlug = Gaffer.IntPlug()
node.stringPlug = Gaffer.StringPlug()

column.pack_start( GafferUI.PlugWidget( node.floatPlug, "Float", "I am a description" ).getGTKWidget(), False )
column.pack_start( GafferUI.PlugWidget( node.intPlug, "Int", "I am a description too" ).getGTKWidget(), False )
column.pack_start( GafferUI.PlugWidget( node.stringPlug ).getGTKWidget(), False )
column.pack_start( GafferUI.PlugWidget( GafferUI.FileNamePlugValueWidget( node.stringPlug ) ).getGTKWidget(), False )

window.show_all()
gtk.main()
