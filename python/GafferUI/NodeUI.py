from __future__ import with_statement

import Gaffer
import GafferUI

## This class forms a useful base for creating node uis for use in the NodeEditor
# and elsewhere. It provides simple methods for building a ui structured using tabs
# and collapsible elements, and allows customisation of the widget types used for
# each Plug.
class NodeUI( GafferUI.Widget ) :

	def __init__( self, node ) :
	
		self._currentColumn = GafferUI.ListContainer( GafferUI.ListContainer.Orientation.Vertical )
		GafferUI.Widget.__init__( self, self._currentColumn.gtkWidget() )
	
		self.__node = node
		
		self._build()
	
	## Returns the node the ui is being created for.
	def _node( self ) :
	
		return self.__node

	def _tab( self, label ) :
	
		raise NotImplementedError
		
	def _collapsible( self, label ) :
	
		class CollapsibleContext() :
		
			def __init__( self, nodeUI ) :
			
				self.__nodeUI = nodeUI
				self.__label = label
		
			def __enter__( self ) :
			
				cl = GafferUI.Collapsible( self.__label, collapsed=True )
				co = GafferUI.ListContainer( GafferUI.ListContainer.Orientation.Vertical )
				cl.setChild( co )
				
				self.__prevColumn = self.__nodeUI._currentColumn
				
				self.__nodeUI._currentColumn.append( cl )
				self.__nodeUI._currentColumn = co
				
			def __exit__( self, type, value, traceBack ) :
			
				self.__nodeUI._currentColumn = self.__prevColumn
				
		return CollapsibleContext( self )
		
	def _addWidget( self, widget ) :
		
		self._currentColumn.append( widget )
		
	def _addPlugWidget( self, plugOrPlugPath ) :
	
		if isinstance( plugOrPlugPath, basestring ) :
			plug = self._node().getChild( plugOrPlugPath )
		else :
			plug = plugOrPlugPath
		
		vw = GafferUI.PlugValueWidget.create( plug )
		if vw :	
			w = GafferUI.PlugWidget( vw )
			self._addWidget( w )

	## This method is called from the constructor to build the ui. It is
	# intended to be overriden in derived classes.
	def _build( self ) :
		
		self.__buildWalk( self._node() )

	def __buildWalk( self, parent ) :
	
		plugs = [ x for x in parent.children() if x.isInstanceOf( Gaffer.Plug.staticTypeId() ) ]
		plugs = [ x for x in plugs if x.direction()==Gaffer.Plug.Direction.In and not x.getName().startswith( "__" ) ]
		for plug in plugs :

			if plug.typeId()==Gaffer.CompoundPlug.staticTypeId() :
			
				with self._collapsible( plug.getName() ) :
					self.__buildWalk( plug )
				
			else :
			
				self._addPlugWidget( plug )

	@staticmethod
	def _registerPlugWidget( self, nodeTypeId, plugPath, widgetType, **kw ) :
	
		raise NotImplementedError
		
GafferUI.NodeEditor.registerNodeUI( Gaffer.Node.staticTypeId(), NodeUI )
