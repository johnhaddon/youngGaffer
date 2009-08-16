from __future__ import with_statement

import IECore

import Gaffer
import GafferUI

## Returns a menu definition used for the creation of nodes. This is
# initially empty but is expected to be populated during the gaffer
# startup routine.
def definition() :

	return __definition

__definition = IECore.MenuDefinition()

## Utility function to append a menu item to definition.
# nodeCreator must be a callable that returns a Gaffer.Node.	
def append( path, nodeCreator ) :

	definition().append( path, { "command" : __creatorWrapper( nodeCreator=nodeCreator ) } )
	
def __creatorWrapper( nodeCreator ) :

	def f( menu ) :
	
		scriptWindow = menu.ancestor( GafferUI.ScriptWindow )
		script = scriptWindow.getScript()

		node = nodeCreator()

		with Gaffer.UndoContext( script ) :
			script.addChild( node )
			
		script.selection().clear()
		script.selection().add( node )

	return f
