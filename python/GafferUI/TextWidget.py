import pango
import gtk

import GafferUI

class TextWidget( GafferUI.Widget ) :

	def __init__( self ) :
	
		GafferUI.Widget.__init__( self, gtk.Entry() )

		self.gtkWidget().connect( "changed", self.__textChanged )

	def setText( self, text ) :
	
		self.gtkWidget().set_text( text )
		
	def getText( self ) :
	
		return self.gtkWidget().get_text()

	def textChangedSignal( self ) :
	
		try :
			return self.__textChangedSignal
		except :
			self.__textChangedSignal = GafferUI.WidgetSignal()
			
		return self.__textChangedSignal

	## Returns the character index underneath the specified
	# gtkEvent.
	def _eventPosition( self, gtkEvent ) :
	
		layout = self.gtkWidget().get_layout()
		offset = self.gtkWidget().get_layout_offsets()
		cursor = layout.xy_to_index(
			int( (gtkEvent.x - offset[0]) * pango.SCALE ),
			int( (gtkEvent.y - offset[1]) * pango.SCALE )
		)

		return cursor[0] + 1

	def __textChanged( self, gtkEntry ) :
	
		try :
			signal = self.__textChangedSignal
		except :
			return
			
		signal( self )		
		
