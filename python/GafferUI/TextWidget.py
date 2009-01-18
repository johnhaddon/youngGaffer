import pango
import gtk
import IECore

import GafferUI

class TextWidget( GafferUI.Widget ) :

	def __init__( self, text="" ) :
	
		GafferUI.Widget.__init__( self, gtk.Entry() )

		self.gtkWidget().connect( "changed", self.__textChanged )
		self.gtkWidget().connect( "activate", self.__activate )

		self.setText( text )

		self._setColors( self.gtkWidget(), self.State.Normal, self._textEntryFGColor, self._textEntryBGColor )

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

	## A signal emitted when enter is pressed.
	def activatedSignal( self ) :
	
		try :
			return self.__activatedSignal
		except :
			self.__activatedSignal = GafferUI.WidgetSignal()
			
		return self.__activatedSignal

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
		
	def __activate( self, gtkEntry ) :
	
		try :
			signal = self.__activatedSignal
		except :
			return
			
		signal( self )		
	
