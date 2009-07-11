import os

import IECore

import Gaffer
import GafferUI

## Returns a menu definition for the creation of SLO shader nodes.
# This contains entries for all shaders found on the shader searchpath
# defined by DL_SHADERS_PATH.
def definition() :

	if not len( __definition.items() ) :
	
		paths = os.environ.get( "DL_SHADERS_PATH", "" ).split( ":" )
		for path in paths :
			if path in ( ".", "./" ) :
				continue
			for root, dirs, files in os.walk( path ) :
				for f in files :
					s = os.path.splitext( f ) 
					if s[1]==".sdl" :
						shaderName = os.path.join( root[len(path):], s[0] )
						__definition.append( shaderName, { "command" : __createCommand( shaderName ) } )
					
	return __definition

__definition = IECore.MenuDefinition()

def __createCommand( shaderName ) : 
	
	def f( menu ) :
	
		scriptWindow = menu.ancestor( GafferUI.ScriptWindow )
		script = scriptWindow.getScript()

		node = Gaffer.SLONode( name=os.path.split( shaderName )[-1] )
		node["name"].setValue( shaderName )

		with Gaffer.UndoContext( script ) :
			script.addChild( node )

	return f
