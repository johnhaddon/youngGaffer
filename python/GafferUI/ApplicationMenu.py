import os
import IECore
import Gaffer
import GafferUI

def appendDefinitions( menuDefinition, prefix ) :

	menuDefinition.append( prefix + "/Preferences...", { } )
	menuDefinition.append( prefix + "/Documentation...", { "command" : IECore.curry( GafferUI.showURL, os.path.expandvars( "$GAFFER_ROOT/doc/gaffer/html/index.html" ) ) } )
	menuDefinition.append( prefix + "/Quit", { "command" : quit } )
		
def quit( menu ) :

	scriptWindow = menu.ancestor( GafferUI.ScriptWindow )
	application = scriptWindow.getScript().ancestor( Gaffer.ApplicationRoot.staticTypeId() )

	## \todo Check scripts aren't modified
	for script in application["scripts"].children() :
		application["scripts"].removeChild( script )
