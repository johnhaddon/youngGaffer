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
#
## \todo I'm not entirely sure this should derive from
# StringPlugValueWidget and not just PlugValueWidget
## \todo And does it really have it's own button?
## \todo Filter on extension, file or dir etc
class FileNamePlugValueWidget( StringPlugValueWidget ) :

	def __init__( self, plug ) :
	
		StringPlugValueWidget.__init__( self, plug )

		self.gtkWidget = gtk.HBox()
		self.gtkRow = self.gtkWidget
		self.gtkButton = gtk.Button()

		self.gtkRow.pack_start( self.gtkEntry, False )
		self.gtkRow.pack_start( self.gtkButton, False )
		
		self.gtkEntry.connect( "key-press-event", self.__keyPress )
		self.gtkEntry.connect( "button-press-event", self.__buttonPress )

	def __keyPress( self, widget, event ) :
	
		# do tab completion
		if event.keyval==65289 :

			position = self.gtkEntry.get_position()
			
			text = self.gtkEntry.get_text()
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
				
				self.gtkEntry.set_text( newText )
				self.gtkEntry.set_position( newPosition )

			return True	
		
		return False
		
	def __buttonPress( self, widget, event ) :
		
		if event.button==1 and event.state & gtk.gdk.CONTROL_MASK :
		
			# ctrl left click
			
			## \todo Put this code somewhere so it can be shared with
			# the NumericPlugValueWidget code
			layout = self.gtkEntry.get_layout()
			offset = self.gtkEntry.get_layout_offsets()
			cursor = layout.xy_to_index(
				int( (event.x - offset[0]) * pango.SCALE ),
				int( (event.y - offset[1]) * pango.SCALE )
			)
			
			position = cursor[0] + 1
			text = self.gtkEntry.get_text()
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
				## \todo Add a method to the Menu class to do this for us
				m.gtkWidget.popup( None, None, None, event.button, event.time )
			
		return False
		
	def __replacePathEntry( self, position, newEntry ) :
	
		text = self.gtkEntry.get_text()
		prefix = os.path.dirname( text[:position] )
		suffixStartIndex = text.find( "/", position )
		if suffixStartIndex!=-1 :
			suffix = text[suffixStartIndex+1:]
		else :
			suffix = ""
			
		path = os.path.join( prefix, newEntry, suffix )
		while not os.path.exists( path ) :
			path = os.path.dirname( path )
			
		self.gtkEntry.set_text( path )
		self.gtkEntry.set_position( position )
		
