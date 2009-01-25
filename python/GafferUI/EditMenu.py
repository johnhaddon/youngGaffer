import Gaffer
import GafferUI

def appendDefinitions( menuDefinition, prefix="" ) :

	## \todo Grey out options when they won't work.
	menuDefinition.append( prefix + "/Undo", { "command" : undo } )
	menuDefinition.append( prefix + "/Redo", { "command" : redo } )

	## \todo add the following menu items
	#			
	#			(	"/Edit/Cut", {} 	),
	#			(	"/Edit/Copy", {} 	),
	#			(	"/Edit/Paste", {} 	),
	#			(	"/Edit/Delete", {} 	),
	#			(	"/Edit/Select All", {} 	),

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
	
