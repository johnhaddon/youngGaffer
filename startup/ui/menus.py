import os

import IECore
import Gaffer
import GafferUI

scriptWindowMenu = GafferUI.ScriptWindow.menuDefinition()

GafferUI.ApplicationMenu.appendDefinitions( scriptWindowMenu, prefix="/Gaffer" )

GafferUI.FileMenu.appendDefinitions( scriptWindowMenu, prefix="/File" )
GafferUI.EditMenu.appendDefinitions( scriptWindowMenu, prefix="/Edit" )
GafferUI.LayoutMenu.appendDefinitions( scriptWindowMenu, name="/Layout" )

GafferUI.NodeMenu.append( "/File/Read", Gaffer.ReadNode )
GafferUI.NodeMenu.append( "/Primitive/Sphere", Gaffer.SphereNode )
GafferUI.NodeMenu.append( "/Group", Gaffer.GroupNode )
GafferUI.NodeMenu.definition().append( "/Shader/", { "subMenu" : GafferUI.SLOMenu.definition() } )
