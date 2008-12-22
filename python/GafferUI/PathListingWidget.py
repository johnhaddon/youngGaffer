import time

import gobject
import gtk

import GafferUI

## \todo Make columns configurable.
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
		
		print "UPDATE"
				
		children = self.__dirPath().children()
		self.__listView.get_selection().unselect_all()
		self.__listStore.clear()
		print "CLEARED"
		for child in children :
		
			info = child.info() or {}
		
			it = self.__listStore.append()
		
			self.__listStore.set( it, 0, child[-1] )
			self.__listStore.set( it, 1, info.get( "fileSystem:owner", "" ) )
			
			mTime = info.get( "fileSystem:modificationTime", 0 )
			self.__listStore.set( it, 2, mTime )
			self.__listStore.set( it, 3, time.ctime( mTime ) )
		
		print "ADDED"
		self.__currentDir = dirPath
		
	def __dirPath( self ) :
	
		dirPath = self.__path.copy()
		if dirPath.isLeaf() :
			del dirPath[-1]
			
		return dirPath

	def __selectionChanged( self, selection ) :
	
		assert( selection.get_tree_view() is self.__listView )
		
		print "SELECTION CHANGED"
		
		selectionIter = selection.get_selected()[1]
		if not selectionIter :
			return

		selectedName = self.__listStore.get_value( selectionIter, 0 )
		print "SELECT", selectedName
			
		newPath = self.__dirPath()
		newPath.append( selectedName )
				
		self.__path[:] = newPath[:]
		
		print "COMPLETED SELECTION"
		
	def __rowActivated( self, treeView, path, column ) :
	
		print "ROW ACTIVATED"
		self.pathSelectedSignal()( self )		
					
	def __pathChanged( self, path ) :
		
		print "PATH", self.__path
		
		self.__update()
