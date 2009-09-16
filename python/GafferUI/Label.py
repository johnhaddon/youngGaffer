import gtk
import GafferUI

class Label( GafferUI.Widget ) :

	def __init__( self, text="", font={} ) :
	
		GafferUI.Widget.__init__( self, gtk.Label( text ) )
		
		#self.gtkWidget().set_alignment( 0, 0.5 )
		#self.gtkWidget().set_line_wrap( True )
		#self.gtkWidget().set_max_width_chars( 500 )

		self.setFont( **font )

	def setFont( self, **kw ) :
	
		text = self.gtkWidget().get_text()
		font = self._pangoFont( **kw )
		self.gtkWidget().set_markup( "<span %s>%s</span>" % ( font, text ) )
		self.__font = font
		
	def getFont( self, **kw ) :
	
		return self.__font
