import IECore
import Gaffer
import GafferUI
import IECore
import gtk
		
window = GafferUI.Window( title="GadgetWidget test" )
window.gtkWidget().connect( "delete_event", gtk.main_quit )

s = GafferUI.GadgetWidget( GafferUI.NodeGadget() )
window.setChild( s )

window.show()

gtk.main()
