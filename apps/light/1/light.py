import IECore
import Gaffer
import GafferUI
import gtk

class light( Gaffer.Application ) :

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
	
		application = Gaffer.ApplicationRoot()
		GafferUI.ScriptWindow.connect( application )
		
		application["scripts"]["script1"] = Gaffer.ScriptNode()
		
		GafferUI.EventLoop.start()
		
		return 0
