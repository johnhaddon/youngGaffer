import Gaffer
import GafferUI

scriptWindowMenu = GafferUI.ScriptWindow.menuDefinition()

GafferUI.FileMenu.appendDefinitions( scriptWindowMenu, prefix="/File" )

## \todo Make the following menu items in an EditMenu file and append them here
#			
#			(	"/Edit/Undo", {} 	),
#			(	"/Edit/Redo", {} 	),
#			(	"/Edit/UndoDivider", { "divider" : True } 	),
#			(	"/Edit/Cut", {} 	),
#			(	"/Edit/Copy", {} 	),
#			(	"/Edit/Paste", {} 	),
#			(	"/Edit/Delete", {} 	),
#			(	"/Edit/Select All", {} 	),
#
#			(	"/Layout", {} 	),
