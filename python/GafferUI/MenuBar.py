import gtk

from Widget import Widget
from Menu import Menu

class MenuBar( Widget ) :

	def __init__( self, definition ) :
	
		Widget.__init__( self, gtk.MenuBar() )
		
		self.__menuBar = self.gtkWidget()
		
		self.definition = definition
		
	def __setattr__( self, key, value ) :
	
		self.__dict__[key] = value
		if key=="definition" :
		
			for c in self.__menuBar.get_children() :
				self.__menuBar.remove( c )
			self.__subMenus = []

			done = set()
			for path, item in self.definition.items() :
			
				pathComponents = path.strip( "/" ).split( "/" )
				name = pathComponents[0]
				if not name in done :
				
					if len( pathComponents ) > 1 :
						subMenuDefinition = self.definition.reRooted( "/" + name )
					else :
						subMenuDefinition = item.subMenu or IECore.MenuDefinition()
				
					menuItem = gtk.MenuItem( label = name )
					menu = Menu( subMenuDefinition )
					menuItem.set_submenu( menu.gtkWidget() )
					menuItem.show()
					self.__menuBar.append( menuItem )
					self.__subMenus.append( menu )
					
				done.add( name )	
					
