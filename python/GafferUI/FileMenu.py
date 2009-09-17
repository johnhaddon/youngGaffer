import re

import Gaffer
import GafferUI

## Appends items to the IECore.MenuDefinition object passed to build a File menu containing
# standard open/save/revert/etc
def appendDefinitions( menuDefinition, prefix="" ) :

	menuDefinition.append( prefix + "/New", { "command" : new } )
	menuDefinition.append( prefix + "/Open...", { "command" : open } )
	menuDefinition.append( prefix + "/OpenDivider", { "divider" : True } )
	menuDefinition.append( prefix + "/Save", { "command" : save }	)
	menuDefinition.append( prefix + "/Save As...", { "command" : saveAs }	)
	menuDefinition.append( prefix + "/Revert To Saved", { "command" : revertToSaved }	)

## A function suitable as the command for a File/New menu item. It must be invoked from a menu which
# has a ScriptWindow in its ancestry.
def new( menu ) :

	scriptWindow = menu.ancestor( GafferUI.ScriptWindow )
	application = scriptWindow.getScript().ancestor( Gaffer.ApplicationRoot.staticTypeId() )

	newScript = Gaffer.ScriptNode( "script" )
	application["scripts"].addChild( newScript )

## A function suitable as the command for a File/Open menu item. It must be invoked from a menu which
# has a ScriptWindow in its ancestry.
def open( menu ) :

	scriptWindow = menu.ancestor( GafferUI.ScriptWindow )
	
	path = Gaffer.FileSystemPath( "/" )
	path.addFilter( Gaffer.FileNamePathFilter( [ "*.gfr" ] ) )
	path.addFilter( Gaffer.FileNamePathFilter( [ re.compile( "^[^.].*" ) ], leafOnly=False ) )
	dialogue = GafferUI.PathChooserDialogue( path, title="Open script", confirmLabel="Open" )
	scriptWindow.addChildWindow( dialogue )
	path = dialogue.waitForPath()
	dialogue.close()		

	if not path :
		return

	currentScript = scriptWindow.getScript()
	application = scriptWindow.getScript().ancestor( Gaffer.ApplicationRoot.staticTypeId() )
	
	currentNodes = [ n for n in currentScript.children() if n.isInstanceOf( Gaffer.Node.staticTypeId() ) ]
	if not currentNodes and not currentScript["fileName"].getValue() :
		script = currentScript
	else :	
		script = Gaffer.ScriptNode()

	script["fileName"].setValue( str( path ) )
	script.load()

	application["scripts"].addChild( script )

## A function suitable as the command for a File/Save menu item. It must be invoked from a menu which
# has a ScriptWindow in its ancestry.
def save( menu ) :

	scriptWindow = menu.ancestor( GafferUI.ScriptWindow )
	script = scriptWindow.getScript()
	if script["fileName"].getValue() :
		script.save()
	else :
		saveAs( menu )

## A function suitable as the command for a File/Save As menu item. It must be invoked from a menu which
# has a ScriptWindow in its ancestry.
def saveAs( menu ) :

	scriptWindow = menu.ancestor( GafferUI.ScriptWindow )
	script = scriptWindow.getScript()

	path = Gaffer.FileSystemPath( script["fileName"].getValue() )
	path.addFilter( Gaffer.FileNamePathFilter( [ "*.gfr" ] ) )
	path.addFilter( Gaffer.FileNamePathFilter( [ re.compile( "^[^.].*" ) ], leafOnly=False ) )

	dialogue = GafferUI.PathChooserDialogue( path, title="Save script", confirmLabel="Save" )
	scriptWindow.addChildWindow( dialogue )
	path = dialogue.waitForPath()
	dialogue.close()		

	if not path :
		return

	script["fileName"].setValue( str( path ) )
	script.save()

## A function suitable as the command for a File/Revert To Saved menu item. It must be invoked from a menu which
# has a ScriptWindow in its ancestry.
def revertToSaved( menu ) :

	scriptWindow = menu.ancestor( GafferUI.ScriptWindow )
	script = scriptWindow.getScript()

	if script["fileName"].getValue() :
		script.load()
	else :
		## \todo Warn
		pass

# /File/Exit
#############################################################################################

## A function suitable as the command for a File/Exit menu item. It must be invoked from a menu which
# has a ScriptWindow in its ancestry.
## \todo Implement me
def exit( menu ) :

	raise NotImplementedError
