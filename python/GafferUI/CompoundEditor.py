import IECore
import gtk

import GafferUI

## \todo Implement an option to float in a new window, and an option to anchor back - drag and drop of tabs?
class CompoundEditor( GafferUI.EditorWidget ) :

	def __init__( self, scriptNode=None, children=None ) :
	
		# need to create this before constructing base class as we need it for
		# the call the base class will make to setScriptNode during construction.
		self.__splittable = GafferUI.Splittable()
		
		GafferUI.EditorWidget.__init__( self, gtk.EventBox(), scriptNode )
			
		self.gtkWidget().add( self.__splittable.gtkWidget() )
		self.__splittable.gtkWidget().connect( "button-press-event", self.__buttonPress )

		if children :
		
			self.__addChildren( self.__splittable, children )
			
	def setScriptNode( self, scriptNode ) :
	
		GafferUI.EditorWidget.setScriptNode( self, scriptNode )
		
		def __set( w, scriptNode ) :
		
			if isinstance( w, GafferUI.Splittable ) :
		
				if w.isSplit() :
					__set( w.subPanel( 0 ), scriptNode )
					__set( w.subPanel( 1 ), scriptNode )
				else :
					tabbedContainer = w.getChild()
					if tabbedContainer :
						for c in tabbedContainer :
							c.setScriptNode( scriptNode )
						
		__set( self.__splittable, scriptNode )
	
	def __repr__( self ) :
	
		def __serialise( w ) :
		
			if isinstance( w, GafferUI.Splittable ) :
		
				if w.isSplit() :
					return "( GafferUI.Splittable.SplitDirection.%s, ( %s, %s ) )" % ( str( w.splitDirection() ), __serialise( w.subPanel( 0 ) ), __serialise( w.subPanel( 1 ) ) )

				else :
					return "( GafferUI.Splittable.SplitDirection.None, %s )" % __serialise( w.getChild() )
			
			elif isinstance( w, GafferUI.TabbedContainer ) :
			
				r = repr( tuple( w[:] ) )			
				return r
				
			else :
			
				return repr( w )
		
		return "GafferUI.CompoundEditor( children = %s )" % __serialise( self.__splittable )

	def __buttonPress( self, gtkWidget, event ) :
	
		splittable = GafferUI.Widget.owner( gtkWidget )
			
		if splittable.isSplit() :
			## Can only do things at the leaf level
			return False
			
		if event.button==3 :
		
			# right click. show a menu to allow editing of the ui.
			
			m = IECore.MenuDefinition()
			
			for c in GafferUI.EditorWidget.types() :
				m.append( "/" + GafferUI.CamelCase.toSpaced( c ), { "command" : IECore.curry( self.__addChild, splittable, c ) } )

			m.append( "/divider", { "divider" : True } )

			removeItemsAdded = 0
			
			splittableParent = splittable.parent()
			if isinstance( splittableParent, GafferUI.Splittable ) :
				m.append( "Remove Panel", { "command" : IECore.curry( splittableParent.join, 1 - splittableParent.subPanelIndex( splittable ) ) } )
				removeItemsAdded += 1
				
			tabbedContainer = splittable.getChild()
			if tabbedContainer :
				currentTab = tabbedContainer.getCurrent()
				if currentTab :
					m.append( "/Remove " + tabbedContainer.getLabel( currentTab ), { "command" : IECore.curry( tabbedContainer.removeChild, currentTab ) } )
					removeItemsAdded += 1
			
			if removeItemsAdded :		
				m.append( "/divider2", { "divider" : True } )

			m.append( "/Split Left", { "command" : IECore.curry( self.__split, splittable, GafferUI.Splittable.SplitDirection.Vertical, 1 ) } )
			m.append( "/Split Right", { "command" : IECore.curry( self.__split, splittable, GafferUI.Splittable.SplitDirection.Vertical, 0 ) } )
			m.append( "/Split Bottom", { "command" : IECore.curry( self.__split, splittable, GafferUI.Splittable.SplitDirection.Horizontal, 0 ) } )
			m.append( "/Split Top", { "command" : IECore.curry( self.__split, splittable, GafferUI.Splittable.SplitDirection.Horizontal, 1 ) } )

			m = GafferUI.Menu( m )
			m.popup()
			return True
		
		return False

	def __addChildren( self, splittable, children ) :
		
		if children[0]==GafferUI.Splittable.SplitDirection.None :
			
			if children[1] :
				for c in children[1] :
					self.__addChild( splittable, c )
					
		else :
		
			self.__split( splittable, children[0], 0 )
			self.__addChildren( splittable.subPanel( 0 ), children[1][0] )
			self.__addChildren( splittable.subPanel( 1 ), children[1][1] )

	def __addChild( self, splittable, nameOrEditor ) :
	
		assert( not splittable.isSplit() )
		
		tabbedContainer = splittable.getChild()
		if not tabbedContainer :
			tabbedContainer = GafferUI.TabbedContainer()
			splittable.setChild( tabbedContainer )
		
		if isinstance( nameOrEditor, basestring ) :
			editor = GafferUI.EditorWidget.create( nameOrEditor, self.getScriptNode() )
		else :
			editor = nameOrEditor
			editor.setScriptNode( self.getScriptNode() )
			
		tabbedContainer.append( editor )
		tabbedContainer.setLabel( editor, GafferUI.CamelCase.toSpaced( editor.__class__.__name__ ) )
		
	def __split( self, splittable, direction, subPanelIndex ) :
	
		assert( not splittable.isSplit() )
		
		splittable.split( direction, subPanelIndex )
		splittable.subPanel( 0 ).gtkWidget().connect( "button-press-event", self.__buttonPress )
		splittable.subPanel( 1 ).gtkWidget().connect( "button-press-event", self.__buttonPress )
