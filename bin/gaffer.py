#!/usr/bin/env python

import os
import sys
import IECore

## \todo Modify this to accomodate unversioned classes, so we can lose the subdirectories
# in apps
appLoader = IECore.ClassLoader( IECore.SearchPath( os.environ["GAFFER_APP_PATHS"], ":" ) )

if len( sys.argv ) < 2 :

	sys.stderr.write( "Usage : gaffer app [options]\n\n" )
	sys.stderr.write( "Available applications : \n\n" )
	for a in appLoader.classNames() :
		sys.stderr.write( "\t%s\n" % a )
		
	sys.exit( 1 )
	
app = appLoader.load( sys.argv[1] )()

IECore.ParameterParser().parse( sys.argv[2:], app.parameters() )

result = app.run()
sys.exit( result )
