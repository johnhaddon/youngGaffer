import IECore
import Gaffer
import code

class cli( Gaffer.Application ) :

	def __init__( self ) :
	
		Gaffer.Application.__init__( self )
		
	def doRun( self, args ) :

		console = code.InteractiveConsole()
		console.runsource( "import IECore; import Gaffer" )
		console.interact( "Gaffer" )
		
		return 0
