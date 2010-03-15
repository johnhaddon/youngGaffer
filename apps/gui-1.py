import IECore
import Gaffer
import GafferUI
import gtk
import os

class gui( Gaffer.Application ) :

	def __init__( self ) :
	
		Gaffer.Application.__init__( self )
		
		self.parameters().addParameters(
		
			[
				IECore.StringVectorParameter(
					name = "scripts",
					description = "A list of scripts to edit.",
					defaultValue = IECore.StringVectorData()
				)
			]
			
		)
		
		self.parameters().userData()["parser"] = IECore.CompoundObject(
			{
				"flagless" : IECore.StringVectorData( [ "scripts" ] )
			}
		)
		
	def doRun( self, args ) :
	
		self._executeStartupFiles( [ "ui" ] )
	
		application = Gaffer.ApplicationRoot()
		GafferUI.ScriptWindow.connect( application )
		
		if len( args["scripts"] ) :
			for fileName in args["scripts"] :
				scriptNode = Gaffer.ScriptNode( os.path.splitext( os.path.basename( fileName ) )[0] )
				scriptNode["fileName"].setValue( os.path.abspath( fileName ) )
				scriptNode.load()
				application["scripts"].addChild( scriptNode )
		else :
			application["scripts"]["script1"] = Gaffer.ScriptNode()
				
		GafferUI.EventLoop.start()
		
		return 0
