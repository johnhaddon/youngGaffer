from __future__ import with_statement

import Gaffer
import GafferUI

def appendDefinitions( menuDefinition, prefix="" ) :

	## \todo Grey out options when they won't work.
	menuDefinition.append( prefix + "/Undo", { "command" : undo } )
	menuDefinition.append( prefix + "/Redo", { "command" : redo } )
	menuDefinition.append( prefix + "/UndoDivider", { "divider" : True } )
	
	menuDefinition.append( prefix + "/Cut", { "command" : cut } )
	menuDefinition.append( prefix + "/Copy", { "command" : copy } )
	menuDefinition.append( prefix + "/Paste", { "command" : paste } )
	menuDefinition.append( prefix + "/Delete", { "command" : delete } )
	menuDefinition.append( prefix + "/CutCopyPasteDeleteDivider", { "divider" : True } )

	menuDefinition.append( prefix + "/Select All", { "command" : selectAll } )

## A function suitable as the command for an Edit/Undo menu item. It must
# be invoked from a menu that has a ScriptWindow in its ancestry.
def undo( menu ) :

	scriptWindow = menu.ancestor( GafferUI.ScriptWindow )
	script = scriptWindow.getScript()
	script.undo()
	
## A function suitable as the command for an Edit/Redo menu item. It must
# be invoked from a menu that has a ScriptWindow in its ancestry.
def redo( menu ) :

	scriptWindow = menu.ancestor( GafferUI.ScriptWindow )
	script = scriptWindow.getScript()
	script.redo()

## A function suitable as the command for an Edit/Cut menu item. It must
# be invoked from a menu that has a ScriptWindow in its ancestry.
def cut( menu ) :

	scriptWindow = menu.ancestor( GafferUI.ScriptWindow )
	script = scriptWindow.getScript()

	with Gaffer.UndoContext( script ) :
		script.cut( script.selection() )

## A function suitable as the command for an Edit/Copy menu item. It must
# be invoked from a menu that has a ScriptWindow in its ancestry.
def copy( menu ) :

	scriptWindow = menu.ancestor( GafferUI.ScriptWindow )
	script = scriptWindow.getScript()
	script.copy( script.selection() )

## A function suitable as the command for an Edit/Paste menu item. It must
# be invoked from a menu that has a ScriptWindow in its ancestry.
def paste( menu ) :

	scriptWindow = menu.ancestor( GafferUI.ScriptWindow )
	script = scriptWindow.getScript()
	
	with Gaffer.UndoContext( script ) :
	
		script.paste()
	
## A function suitable as the command for an Edit/Delete menu item. It must
# be invoked from a menu that has a ScriptWindow in its ancestry.
def delete( menu ) :

	scriptWindow = menu.ancestor( GafferUI.ScriptWindow )
	script = scriptWindow.getScript()
	
	with Gaffer.UndoContext( script ) :
		
		script.deleteNodes( script.selection() )
	
## A function suitable as the command for an Edit/Select All menu item. It must
# be invoked from a menu that has a ScriptWindow in its ancestry.
def selectAll( menu ) :

	scriptWindow = menu.ancestor( GafferUI.ScriptWindow )
	script = scriptWindow.getScript()
	
	for c in script.children() :
		if c.isInstanceOf( Gaffer.Node.staticTypeId() ) :
			script.selection().add( c )	
