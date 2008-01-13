import gtk
from IECore import curry
from Widget import Widget

## \todo Decide how this interacts with the other UI components to
# popup when wanted.
class Menu( Widget ) :

	def __init__( self, definition ) :
	
		Widget.__init__( self, gtk.Menu() )
	
		self.definition = definition
		self.__menu = self.gtkWidget()
		
		self.__menu.connect( "show", self.__show, self.definition )
		
	def popup( self ) :
	
		event = gtk.get_current_event()
		self.__menu.popup( None, None, None, event.button, event.time )

	@staticmethod
	def __commandWrapper( command, menuItem ) :
	
		command()

	@staticmethod
	def __show( menu, definition ) :
		
		if callable( definition ) :
			definition = definition()
			
		oldMenuItems = menu.get_children()
		for oldMenuItem in oldMenuItems :
			menu.remove( oldMenuItem )	
			
		done = set()
		for path, item in definition.items() :
		
			pathComponents = path.strip( "/" ).split( "/" )
			name = pathComponents[0]
			
			if not name in done :

				menuItem = None
				if len( pathComponents ) > 1 :
					
					# it's a submenu

					subMenu = gtk.Menu()
					subMenuDefinition = definition.reRooted( "/" + name + "/" )
					subMenu.connect( "show", Menu.__show, subMenuDefinition )

					menuItem = gtk.MenuItem( label = name )
					menuItem.set_submenu( subMenu )
						
				else :
				
					# it's not a submenu
					
					if item.divider :
						
						menuItem = gtk.SeparatorMenuItem()
						
					elif item.subMenu :
					
						subMenu = gtk.Menu()
						subMenu.connect( "show", Menu.__show, item.subMenu )
						menuItem = gtk.MenuItem( label = name )
						menuItem.set_submenu( subMenu )
											
					else :
					
						active = item.active
						if callable( active ) :
							active = active()
							
						menuItem = gtk.MenuItem( label = name )
						menuItem.set_sensitive( active )
						
						if item.command :
						
							menuItem.connect( "activate", curry( Menu.__commandWrapper, item.command ) )

				menuItem.show()
				menu.append( menuItem )
						
				done.add( name )
