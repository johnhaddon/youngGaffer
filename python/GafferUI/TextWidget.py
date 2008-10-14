import GafferUI
import gtk

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

	def __textChanged( self, gtkEntry ) :
	
		try :
			signal = self.__textChangedSignal
		except :
			return
			
		signal( self )		
		
