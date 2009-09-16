import gtk

import IECore

import GafferUI

class URLWidget( GafferUI.Widget ) :

	def __init__( self, url="", label=None, font={} ) :
	
		GafferUI.Widget.__init__( self, gtk.EventBox() )

		self.__label = gtk.Label()
		self.__label.show()

		self.gtkWidget().add_events( gtk.gdk.BUTTON_PRESS_MASK )
		self.gtkWidget().add_events( gtk.gdk.ENTER_NOTIFY_MASK )
		self.gtkWidget().add_events( gtk.gdk.LEAVE_NOTIFY_MASK )
		self.gtkWidget().connect( "button-press-event", self.__buttonPress )
		self.gtkWidget().connect( "enter-notify-event", self.__enterNotify )
		self.gtkWidget().connect( "leave-notify-event", self.__leaveNotify )

		self.gtkWidget().add( self.__label )
		
		self.__url = url
		self.__labelText = label
		self.__font = font
		self.__updateMarkup()

	def setFont( self, **kw ) :
	
		self.__font = kw
		self.__updateMarkup()
		
	def getFont( self ) :
	
		return self.__font

	def getURL( self ) :
	
		return self.__url

	def setURL( self, url ) :
	
		self.__url = url
		self.__updateMarkup()

	def getLabel( self ) :
	
		return self.__labelText
		
	def setLabel( self, label ) :
	
		self.__labelText = label
		self.__updateMarkup()

	def __buttonPress( self, widget, event ) :
	
		GafferUI.showURL( self.getURL() )
		
	def __enterNotify( self, widget, event ) :
	
		self.__label.set_state( gtk.STATE_PRELIGHT )
		self.__updateMarkup()
	
	def __leaveNotify( self, widget, event ) :
	
		self.__label.set_state( gtk.STATE_NORMAL )
		self.__updateMarkup()
		
	def __updateMarkup( self ) :
	
		text = self.getLabel() or self.getURL()
		font = self._pangoFont( **self.__font )
		if self.__label.state==gtk.STATE_PRELIGHT :
			self.__label.set_markup( "<span underline='double' underline_color='#FF0000' " + font + ">" + text + "</span>" )
		else :
			self.__label.set_markup( "<span underline='single' " + font + ">" + text + "</span>" )

