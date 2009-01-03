import Gaffer
import GafferUI

scriptWindowMenu = GafferUI.ScriptWindow.menuDefinition()

GafferUI.FileMenu.appendDefinitions( scriptWindowMenu, prefix="/File" )
GafferUI.LayoutMenu.appendDefinitions( scriptWindowMenu, name="/Layout" )

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
