import GafferUI

import IECore
import gtk

## \todo Implement an option to float in a new window, and an option to anchor back
## \todo Serialisation using __repr__ (requires serialisation of children too) so we can restore layouts from preferences.
class CompoundEditor( GafferUI.EditorWidget ) :

	def __init__( self, scriptNode ) :
	
		GafferUI.EditorWidget.__init__( self, gtk.EventBox(), scriptNode )
			
		self.__splittable = GafferUI.Splittable()
		self.gtkWidget().add( self.__splittable.gtkWidget() )
		self.__splittable.gtkWidget().connect( "button-press-event", self.__buttonPress )

	def __buttonPress( self, gtkWidget, event ) :
	
		splittable = GafferUI.Widget.owner( gtkWidget )
			
		if splittable.isSplit() :
			## Can only do things at the leaf level
			return False
			
		if event.button==3 :
		
			# right click. show a menu to allow editing of the ui.
			
			m = IECore.MenuDefinition()
			
			for c in GafferUI.EditorWidget.__subclasses__() :
				if not c is CompoundEditor :
					m.append( "/" + c.__name__, { "command" : IECore.curry( self.__addChildCallback, splittable, c ) } )

			m.append( "/divider", { "divider" : True } )

			removeItemsAdded = 0
			
			splittableParent = splittable.parent()
			if isinstance( splittableParent, GafferUI.Splittable ) :
				m.append( "removePanel", { "command" : IECore.curry( splittableParent.join, 1 - splittableParent.subPanelIndex( splittable ) ) } )
				removeItemsAdded += 1
				
			tabbedContainer = splittable.getChild()
			if tabbedContainer :
				currentTab = tabbedContainer.getCurrent()
				if currentTab :
					m.append( "/remove " + tabbedContainer.getLabel( currentTab ), { "command" : IECore.curry( tabbedContainer.removeChild, currentTab ) } )
					removeItemsAdded += 1
			
			if removeItemsAdded :		
				m.append( "/divider2", { "divider" : True } )

			m.append( "/splitLeft", { "command" : IECore.curry( self.__splitCallback, splittable, GafferUI.Splittable.SplitDirection.Vertical, 1 ) } )
			m.append( "/splitRight", { "command" : IECore.curry( self.__splitCallback, splittable, GafferUI.Splittable.SplitDirection.Vertical, 0 ) } )
			m.append( "/splitBottom", { "command" : IECore.curry( self.__splitCallback, splittable, GafferUI.Splittable.SplitDirection.Horizontal, 0 ) } )
			m.append( "/splitTop", { "command" : IECore.curry( self.__splitCallback, splittable, GafferUI.Splittable.SplitDirection.Horizontal, 1 ) } )

			m = GafferUI.Menu( m )
			m.popup()
			return True
		
		return False

	def __addChildCallback( self, splittable, cls ) :
	
		assert( not splittable.isSplit() )
	
		tabbedContainer = splittable.getChild()
		if not tabbedContainer :
			tabbedContainer = GafferUI.TabbedContainer()
			splittable.setChild( tabbedContainer )
		
		newEditor = cls( self.scriptNode() )
		tabbedContainer.append( newEditor )
		tabbedContainer.setLabel( newEditor, cls.__name__ )
		
	def __splitCallback( self, splittable, direction, subPanelIndex ) :
	
		assert( not splittable.isSplit() )
		
		splittable.split( direction, subPanelIndex )
		splittable.subPanel( 0 ).gtkWidget().connect( "button-press-event", self.__buttonPress )
		splittable.subPanel( 1 ).gtkWidget().connect( "button-press-event", self.__buttonPress )
