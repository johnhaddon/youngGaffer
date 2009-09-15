import os

import GafferUI

class AboutWindow( GafferUI.Window ) :

	def __init__( self, about ) :
	
		GafferUI.Window.__init__( self, title = "About " + about.name() )
		
		frame = GafferUI.Frame( borderWidth = 30 )
		column = GafferUI.ListContainer( GafferUI.ListContainer.Orientation.Vertical, spacing=10 )
		frame.setChild( column )
		
		name = GafferUI.Label( text = about.name() + " " + about.versionString() )
		column.append( name )
				
		copy = GafferUI.Label( text = about.copyright() )
		column.append( copy )
		
		url = GafferUI.URLWidget( about.url() )
		column.append( url )
		
		dependencies = about.dependencies()
		if dependencies :
		
			collapsible = GafferUI.Collapsible( label="Dependencies", collapsed=True )
			scrollable = GafferUI.ScrolledContainer(
				horizontalMode=GafferUI.ScrolledContainer.ScrollMode.Never,
				verticalMode=GafferUI.ScrolledContainer.ScrollMode.Always,
				borderWidth = 5
			)
			depColumn = GafferUI.ListContainer( GafferUI.ListContainer.Orientation.Vertical, spacing=5, borderWidth=10 )
			scrollable.setChild( depColumn )
			collapsible.setChild( scrollable )
			
			depColumn.append( GafferUI.Label(
				about.name() + " includes code from several open source projects.\n"
				"Specific licensing information, credits, downloads and\n"
				"urls are provided for each project below."
			) )
			
			for d in dependencies :
			
				name = GafferUI.Label( text = d["name"] )
				depColumn.append( name )
				
				if "license" in d :
					license = GafferUI.URLWidget( url="file://" + os.path.expandvars( d["license"] ), label="License" )
					depColumn.append( license )
					
				if "url" in d :
					url = GafferUI.URLWidget( d["url"] )
					depColumn.append( url )
					
				if "download" in d :
					download = GafferUI.URLWidget( url=d["download"], label="Download" )
					depColumn.append( download )
			
			column.append( collapsible, expand=True )

		self.setChild( frame )

