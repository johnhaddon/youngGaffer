import time

import gobject
import gtk

import Gaffer
import GafferUI

## \todo Make columns configurable.
## \todo Select appropriate row when path represents a file (for instance because it has been entered in the text field manually).
class PathListingWidget( GafferUI.Widget ) :

	def __init__( self, path ) :
	
		GafferUI.Widget.__init__( self, gtk.EventBox() )
		
		self.__scroller = GafferUI.ScrolledContainer()
		self.gtkWidget().add( self.__scroller.gtkWidget() )
		
		self.__listStore = gtk.ListStore( gobject.TYPE_STRING, gobject.TYPE_STRING, gobject.TYPE_FLOAT, gobject.TYPE_STRING )

		self.__listView = gtk.TreeView( self.__listStore )
		self.__listView.get_selection().set_mode( gtk.SELECTION_SINGLE )
		self.__scroller.setChild( GafferUI.Widget( self.__listView ) )

		nameColumn = gtk.TreeViewColumn( "Name", gtk.CellRendererText(), text=0 )
		nameColumn.set_sort_column_id( 0 )
		self.__listView.append_column( nameColumn )
		
		userColumn = gtk.TreeViewColumn( "User", gtk.CellRendererText(), text=1 )
		userColumn.set_sort_column_id( 1 )
		self.__listView.append_column( userColumn )
		
		userColumn = gtk.TreeViewColumn( "Modification Date", gtk.CellRendererText(), text=3 )
		userColumn.set_sort_column_id( 2 )
		self.__listView.append_column( userColumn )
		
		selection = self.__listView.get_selection()
		selection.connect( "changed", self.__selectionChanged )
		
		self.__listView.connect( "row-activated", self.__rowActivated )
		
		self.__path = path
		self.__pathChangedConnection = self.__path.pathChangedSignal().connect( self.__pathChanged )
		
		self.__currentDir = None
		self.__update()
	
		self.__pathSelectedSignal = GafferUI.WidgetSignal()
	
	## This signal is emitted when the user double clicks on a leaf path.
	def pathSelectedSignal( self ) :
	
		return self.__pathSelectedSignal
			
	def __update( self ) :
				
		dirPath = self.__dirPath()
		if self.__currentDir==dirPath :
			return
						
		children = dirPath.children()
		self.__listStore.clear()
		for child in children :
		
			info = child.info() or {}
		
			it = self.__listStore.append()
		
			self.__listStore.set( it, 0, child[-1] )
			self.__listStore.set( it, 1, info.get( "fileSystem:owner", "" ) )
			
			mTime = info.get( "fileSystem:modificationTime", 0 )
			self.__listStore.set( it, 2, mTime )
			self.__listStore.set( it, 3, time.ctime( mTime ) )
		
		self.__currentDir = dirPath
		
	def __dirPath( self ) :
	
		p = self.__path.copy()
		if p.isLeaf() :
			# if it's a leaf then take the parent
			del p[-1]
		else :
			# it's not a leaf.
			if not p.isValid() :
				# it's not valid. if we can make it
				# valid by trimming the last element
				# then do that
				pp = p.copy()
				del pp[-1]
				if pp.isValid() :
					p = pp
			else :
				# it's valid and not a leaf, and
				# that's what we want.
				pass
						
		return p

	def __selectionChanged( self, selection ) :
	
		selectedRows = selection.get_selected_rows()[1]
		if not selectedRows :
			return True
			
		selectionIter = self.__listStore.get_iter( selectedRows[0] )
		selectedName = self.__listStore.get_value( selectionIter, 0 )
					
		newPath = self.__currentDir.copy()
		newPath.append( selectedName )
		if newPath.isLeaf() :
			self.__path[:] = newPath[:]
		
		return True
		
	def __rowActivated( self, treeView, path, column ) :
		
		name = self.__listStore.get_value( self.__listStore.get_iter( path ), 0 )		
		newPath = self.__currentDir.copy()
		newPath.append( name )
		self.__path[:] = newPath[:]
	
		if self.__path.isLeaf() :
			self.pathSelectedSignal()( self )		
		
	def __pathChanged( self, path ) :
		
		self.__update()
