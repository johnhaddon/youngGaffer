import Gaffer
import GafferUI

scriptWindowMenu = GafferUI.ScriptWindow.menuDefinition()

GafferUI.FileMenu.appendDefinitions( scriptWindowMenu, prefix="/File" )
GafferUI.EditMenu.appendDefinitions( scriptWindowMenu, prefix="/Edit" )
GafferUI.LayoutMenu.appendDefinitions( scriptWindowMenu, name="/Layout" )

GafferUI.NodeMenu.append( "/File/Read", Gaffer.ReadNode )
GafferUI.NodeMenu.append( "/Utility/Node", Gaffer.Node )
GafferUI.NodeMenu.append( "/Maths/Add", Gaffer.AddNode )
 
