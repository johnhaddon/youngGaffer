#!/usr/bin/env python

try :

	import os
	import sys
	import IECore

	## \todo Modify this to accomodate unversioned classes, so we can lose the subdirectories
	# in apps
	appLoader = IECore.ClassLoader( IECore.SearchPath( os.environ["GAFFER_APP_PATHS"], ":" ) )

	appName = "gui"
	appArgs = sys.argv[1:]
	if len( sys.argv ) > 1 :
	
		if sys.argv[1] in appLoader.classNames() :
			appName = sys.argv[1]
			appArgs = appArgs[1:]

	app = appLoader.load( appName )()

	IECore.ParameterParser().parse( appArgs, app.parameters() )

	result = app.run()
	sys.exit( result )

except KeyboardInterrupt :

	import sys
	sys.exit( 1 )
	
	
