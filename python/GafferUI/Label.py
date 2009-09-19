import gtk

import IECore

import GafferUI

class Label( GafferUI.Widget ) :

	def __init__( self, text="", font={}, alignment=IECore.V2f( 0.5, 0.5 ) ) :
	
		GafferUI.Widget.__init__( self, gtk.Label( text ) )

		self.setFont( **font )
		self.setAlignment( alignment )

	def setAlignment( self, alignment ) :
	
		self.gtkWidget().set_alignment( alignment[0], alignment[1] )
		
	def getAlignment( self ) :
	
		return IECore.V2f( *self.gtkWidget().get_alignment() )

	def setFont( self, **kw ) :
	
		text = self.gtkWidget().get_text()
		font = self._pangoFont( **kw )
		self.gtkWidget().set_markup( "<span %s>%s</span>" % ( font, text ) )
		self.__font = font
		
	def getFont( self, **kw ) :
	
		return self.__font
