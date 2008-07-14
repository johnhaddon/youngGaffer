import IECore
import Gaffer
import GafferUI
import gtk

class view( Gaffer.Application ) :

	def __init__( self ) :
	
		Gaffer.Application.__init__( self )
		
		self.parameters().addParameters(
		
			[
				IECore.StringVectorParameter(
					name = "files",
					description = "A list of files to view.",
					defaultValue = IECore.StringVectorData()
				)
			]
			
		)
		
		self.parameters().userData()["parser"] = IECore.CompoundObject(
			{
				"flagless" : IECore.StringVectorData( [ "files" ] )
			}
		)
		
	def doRun( self, args ) :
	
		if len( args.files ) < 1 or len( args.files ) > 2 :
		
			raise Exception( "Must view exactly one file." )
	
		o = IECore.Reader.create( args.files[0] ).read()
		
		window = GafferUI.Window( title = "Gaffer Viewer" )
		## \todo Remove gtk from the interface.
		window.gtkWidget().connect( "delete_event", gtk.main_quit )
		viewer = GafferUI.GadgetWidget( GafferUI.RenderableGadget( o ) )
		window.setChild( viewer )
		
		window.show()
		
		GafferUI.EventLoop.start()
		return 0
