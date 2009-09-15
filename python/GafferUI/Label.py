import gtk
import GafferUI

class Label( GafferUI.Widget ) :

	def __init__( self, text="" ) :
	
		GafferUI.Widget.__init__( self, gtk.Label( text ) )
		
		#self.gtkWidget().set_alignment( 0, 0.5 )
		#self.gtkWidget().set_line_wrap( True )
		#self.gtkWidget().set_max_width_chars( 500 )
