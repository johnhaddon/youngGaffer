from PlugValueWidget import PlugValueWidget
from StringPlugValueWidget import StringPlugValueWidget
from Menu import Menu
import gtk
import os.path
import os
import glob
import pango
import IECore

## User docs :
#
# Tab does shell style auto completion.
# Control left click brings up a menu for all options at that path level.
# Return commits any changes onto the plug.
# Escape abandons any uncommitted changes.
#
## \todo Make the button do somethin
## \todo Filter on extension, sequence, file or dir etc
## \todo Use FileNameWidget.
class FileNamePlugValueWidget( PlugValueWidget ) :

	def __init__( self, plug ) :
	
		PlugValueWidget.__init__( self, gtk.HBox(), plug )

		self.__row = self.gtkWidget()
		self.__button = gtk.Button()
		self.__stringValueWidget = StringPlugValueWidget( plug )

		self.__row.pack_start( self.__stringValueWidget.gtkWidget(), False )
		self.__row.pack_start( self.__button, False )
		
		self.__row.connect( "key-press-event", self.__keyPress )
		self.__entry().connect( "button-press-event", self.__buttonPress )
		
		self.__row.show_all()
	
	def updateFromPlug( self ) :
	
		# We don't need to do anything, as the StringPlugValueWidget
		# we hold will do the work
		pass
	
	def __entry( self ) :
	
		return self.__stringValueWidget.gtkWidget()
		
	def __keyPress( self, widget, event ) :
	
		# do tab completion
		if event.keyval==65289 :

			position = self.__entry().get_position()
			
			text = self.__entry().get_text()
			completions = glob.glob( text[:position]+"*" )
			completions = [os.path.basename( x ) for x in completions ]
			completion = os.path.commonprefix( completions )		
		
			if len( completion ) :

				prefix = os.path.dirname( text[:position] )
				suffixStartIndex = text.find( "/", position )
				if suffixStartIndex!=-1 :
					suffix = text[suffixStartIndex+1:]
				else :
					suffix = ""
					
				newText = os.path.join( prefix, completion )
				if len( completions )==1 and os.path.isdir( newText ) :
					newText+="/"
				newPosition = len( newText )
				if len( suffix ) :
					if newText[-1]!='/' :
						newText += "/"
					newText += suffix
				
				self.__entry().set_text( newText )
				self.__entry().set_position( newPosition )

			return True	
		
		return False
		
	def __buttonPress( self, widget, event ) :
				
		if event.button==1 and event.state & gtk.gdk.CONTROL_MASK :
		
			# ctrl left click
			
			## \todo Put this code somewhere so it can be shared with
			# the NumericPlugValueWidget code
			layout = self.__entry().get_layout()
			offset = self.__entry().get_layout_offsets()
			cursor = layout.xy_to_index(
				int( (event.x - offset[0]) * pango.SCALE ),
				int( (event.y - offset[1]) * pango.SCALE )
			)
			
			position = cursor[0] + 1
			text = self.__entry().get_text()
			dir = os.path.dirname( text[:position] )
			if dir=="" :
				dir = "."
			options = glob.glob( dir + "/*" )
			options = [ os.path.basename( x ) for x in options  ]
			
			if len( options ) :
							
				md = IECore.MenuDefinition()
				for o in options :
					md.append( "/" + o,
						IECore.MenuItemDefinition(
							label=o,
							command = IECore.curry( self.__replacePathEntry, position, o )
						)
					)
					
				m = Menu( md )
				m.popup()
			
		return False
		
	def __replacePathEntry( self, position, newEntry ) :
	
		text = self.__entry().get_text()
		prefix = os.path.dirname( text[:position] )
		suffixStartIndex = text.find( "/", position )
		if suffixStartIndex!=-1 :
			suffix = text[suffixStartIndex+1:]
		else :
			suffix = ""
			
		path = os.path.join( prefix, newEntry, suffix )
		while not os.path.exists( path ) :
			path = os.path.dirname( path )
			
		self.__entry().set_text( path )
		self.__entry().set_position( position )
		
