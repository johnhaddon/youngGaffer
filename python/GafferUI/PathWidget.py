from __future__ import with_statement
import os
import glob

import IECore
import gtk

import Gaffer
import GafferUI

class PathWidget( GafferUI.TextWidget ) :

	def __init__( self, path ) :
	
		GafferUI.TextWidget.__init__( self )
		
		self.gtkWidget().connect( "key-press-event", self.__keyPress )
		self.gtkWidget().connect( "button-press-event", self.__buttonPress )
		
		self.__path = path
		self.__pathChangedConnection = self.__path.pathChangedSignal().connect( self.__pathChanged )
		
		self.__textChangedConnection = self.textChangedSignal().connect( self.__textChanged )
		
	def __keyPress( self, widget, event ) :
	
		# do tab completion
		if event.keyval==65289 :

			position = self.gtkWidget().get_position()

			truncatedPath = self.__path.copy()
			truncatedPath.setFromString( str( truncatedPath )[:position] )
			if len( truncatedPath ) :
				matchStart = truncatedPath[-1]
				del truncatedPath[-1]
			else :
				matchStart = ""
				
			matches = [ x[-1] for x in truncatedPath.children() if x[-1].startswith( matchStart ) ]
			match = os.path.commonprefix( matches )
			
			if match :

				self.__path[:] = truncatedPath[:] + [ match ]
				if len( matches )==1 and not self.__path.isLeaf() :
					text = self.getText()
					if not text.endswith( "/" ) :
						self.setText( text + "/" )
				
				self.gtkWidget().set_position( len( self.getText() ) )
						
			return True	
		
		return False
		
	def __buttonPress( self, widget, event ) :
				
		if event.button==1 and event.state & gtk.gdk.CONTROL_MASK :
		
			# ctrl left click
			
			position = self._eventPosition( event )
			dirPath = self.__path.copy()
			n = os.path.dirname( self.getText()[:position] ) or "/"
			dirPath.setFromString( n )
			
			options = dirPath.children()
			options = [ x[-1] for x in options ]
			
			if len( options ) :
							
				md = IECore.MenuDefinition()
				for o in options :
					md.append( "/" + o,
						IECore.MenuItemDefinition(
							label=o,
							command = IECore.curry( self.__replacePathEntry, len( dirPath ), o )
						)
					)
					
				m = GafferUI.Menu( md )
				m.popup()
			
		return False
		
	def __replacePathEntry( self, position, newEntry ) :
	
		if position==len( self.__path ) :
			self.__path.append( newEntry )
		else :
			self.__path[position] = newEntry
			self.__path.truncateUntilValid()			
	
		if position==len( self.__path )-1 and not self.__path.isLeaf() :
			self.setText( self.getText() + "/" )
				
	def __pathChanged( self, path ) :
	
		self.setText( str( path ) )

	def __textChanged( self, widget ) :
	
		text = self.getText()
		with Gaffer.BlockedConnection( self.__pathChangedConnection ) :
			try :
				self.__path.setFromString( self.getText() )
			except :
				# no need to worry too much - it's up to the user to enter
				# something valid. maybe they'll get it right next time.
				pass
