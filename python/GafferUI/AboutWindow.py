import os

import IECore

import GafferUI

class AboutWindow( GafferUI.Window ) :

	def __init__( self, about ) :
	
		GafferUI.Window.__init__( self, title = "About " + about.name() )
		
		frame = GafferUI.Frame( borderWidth = 30 )
		column = GafferUI.ListContainer( GafferUI.ListContainer.Orientation.Vertical, spacing=10 )
		frame.setChild( column )
		
		name = GafferUI.Label( text = about.name() + " " + about.versionString() )
		name.setFont( size=GafferUI.Widget.FontSize.Large, weight=GafferUI.Widget.FontWeight.Bold )
		column.append( name )
				
		copy = GafferUI.Label( text = about.copyright() )
		copy.setFont( size=GafferUI.Widget.FontSize.Medium, weight=GafferUI.Widget.FontWeight.Bold )
		column.append( copy )
		
		url = GafferUI.URLWidget( about.url() )
		column.append( url )
		
		dependencies = about.dependencies()
		if dependencies :
		
			collapsible = GafferUI.Collapsible( label="Dependencies", collapsed=False )
			scrollable = GafferUI.ScrolledContainer(
				horizontalMode=GafferUI.ScrolledContainer.ScrollMode.Never,
				verticalMode=GafferUI.ScrolledContainer.ScrollMode.Always,
				borderWidth = 5
			)
			depColumn = GafferUI.ListContainer( GafferUI.ListContainer.Orientation.Vertical, spacing=5, borderWidth=10 )
			scrollable.setChild( depColumn )
			collapsible.setChild( scrollable )
			
			depColumn.append( GafferUI.Label(
					IECore.StringUtil.wrap(
						about.dependenciesPreamble(),
						60
					),
					alignment = IECore.V2f( 0 ),
				) 
			)
			
			for d in dependencies :
			
				spacer = GafferUI.Label( text = "" )
				depColumn.append( spacer )
			
				name = GafferUI.Label( text = d["name"], alignment=IECore.V2f( 0 ) )
				name.setFont( size=name.FontSize.Medium, weight=name.FontWeight.Bold )
				depColumn.append( name )
				
				if "credit" in d :
					credit = GafferUI.Label( text=IECore.StringUtil.wrap( d["credit"], 60 ), alignment=IECore.V2f( 0 ) )
					depColumn.append( credit )
				
				if "license" in d :
					license = GafferUI.URLWidget( url="file://" + os.path.expandvars( d["license"] ), label="License", alignment=IECore.V2f( 0 ) )
					depColumn.append( license )
					
				if "url" in d :
					url = GafferUI.URLWidget( d["url"], alignment=IECore.V2f( 0 ) )
					depColumn.append( url )
					
				if "source" in d :
					source = GafferUI.URLWidget( url=d["source"], label="Source", alignment=IECore.V2f( 0 ) )
					depColumn.append( source )
			
			column.append( collapsible, expand=True )

		self.setChild( frame )
		
		#self.setResizeable( False )

