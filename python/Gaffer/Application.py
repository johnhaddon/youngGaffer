import IECore

class Application( IECore.Parameterised ) :

	def __init__( self ) :
	
		IECore.Parameterised.__init__( self, "Gaffer.Application", "" )
		pass

	def run( self ) :
	
		args = self.parameters().getValidatedValue()
		return self.doRun( args )
