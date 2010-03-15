import sys
import os

import IECore

import Gaffer

class license( Gaffer.Application ) :

	def __init__( self ) :
	
		Gaffer.Application.__init__( self )
		
		self.parameters().addParameter(
		
			IECore.BoolParameter(
				name = "withDependencies",
				description = "Display the copyright and licensing information for the dependencies.",
				defaultValue = True
			)		
		
		)
		
	def doRun( self, args ) :

		sys.stderr.write( Gaffer.About.name() + " " + Gaffer.About.versionString() + "\n" )
		sys.stderr.write( Gaffer.About.copyright() + "\n" )
		sys.stderr.write( Gaffer.About.url() + "\n" )
		
		if args["withDependencies"].value :
			sys.stderr.write( "\n" + Gaffer.About.dependenciesPreamble() + "\n" )
			for d in Gaffer.About.dependencies() :
		
				sys.stderr.write( "\n" + d["name"] + "\n" )
				sys.stderr.write( "-" * len( d["name"] ) + "\n\n" )

				if "credit" in d :
					sys.stderr.write( d["credit"] + "\n" )
				if "url" in d :
					sys.stderr.write( "Project URL : " + d["url"] + "\n" )
				if "license" in d :
					sys.stderr.write( "License : %s\n" % os.path.expandvars( d["license"] ) )
				if "source" in d :
					sys.stderr.write( "Source : %s\n" % os.path.expandvars( d["source"] ) )
			
		return 0
