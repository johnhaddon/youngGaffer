import IECore
import Gaffer
import GafferUI
import IECore
import gtk

## \todo Rejig this to remove all gtk specific code, and use only GafferUI classes instead

window = gtk.Window( gtk.WINDOW_TOPLEVEL )
window.set_size_request( 200, 100 )
window.set_title( "Script editor test" )
window.connect( "delete_event", gtk.main_quit )

s = GafferUI.ScriptEditor( Gaffer.ScriptNode() )
window.add( s.getGTKWidget() )

window.show()

gtk.main()
