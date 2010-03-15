import os

import IECore

class Application( IECore.Parameterised ) :

	def __init__( self ) :
	
		IECore.Parameterised.__init__( self, "" )

	def run( self ) :
	
		args = self.parameters().getValidatedValue()
		return self.doRun( args )

	def _executeStartupFiles( self, subdirectories ) :
	
		sp = os.environ.get( "GAFFER_STARTUP_PATHS", "" )
		if not sp :
			IECore.msg( IECore.Msg.Level.Warning, "Gaffer.Application._executeStartupFiles", "GAFFER_STARTUP_PATHS environment variable not set" )
			return
	
		sp = IECore.SearchPath( sp, ":" )
		rootPaths = sp.paths
	
		for d in subdirectories :
		
			paths = [ os.path.join( p, d ) for p in rootPaths ]
			spd = IECore.SearchPath( ":".join( paths ), ":" )
			
			IECore.loadConfig( spd, {} )
			
