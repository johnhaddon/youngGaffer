import gtk

import IECore

import GafferUI

class ScrolledContainer( GafferUI.ContainerWidget ) :

	ScrollMode = IECore.Enum.create( "Never", "Always", "Automatic" )

	def __init__( self, horizontalMode=ScrollMode.Automatic, verticalMode=ScrollMode.Automatic, borderWidth=0 ) :
	
		GafferUI.ContainerWidget.__init__( self, gtk.ScrolledWindow() )
		
		self.gtkWidget().set_property( "border-width", borderWidth )
		self.setHorizontalMode( horizontalMode )
		self.setVerticalMode( verticalMode )
		
		self.__child = None
		
	def removeChild( self, child ) :
	
		assert( child is self.__child )
		self.gtkWidget().remove( self.__child.gtkWidget() )
		self.__child = None
		
	def setChild( self, child ) :
	
		if self.__child :
			self.removeChild( self.__child )
		
		needsViewport = True
		try :
			child.gtkWidget().get_property( "hadjustment" )
			needsViewport = False
		except :
			pass
			
		if needsViewport :
			self.gtkWidget().add_with_viewport( child.gtkWidget() )
		else :
			self.gtkWidget().add( child.gtkWidget() )
		
		self.__child = child
	
	def getChild( self ) :
	
		return self.__child
	
	__modesToPolicies = {
		ScrollMode.Never : gtk.POLICY_NEVER,
		ScrollMode.Always : gtk.POLICY_ALWAYS,
		ScrollMode.Automatic : gtk.POLICY_AUTOMATIC,
	}

	__policiesToModes = {
		gtk.POLICY_NEVER : ScrollMode.Never,
		gtk.POLICY_ALWAYS : ScrollMode.Always,
		gtk.POLICY_AUTOMATIC : ScrollMode.Automatic,
	}
		
	def setHorizontalMode( self, mode ) :
	
		p = self.gtkWidget().get_policy()
		self.gtkWidget().set_policy( self.__modesToPolicies[mode], p[1] )

	def getHorizontalMode( self ) :
	
		p = self.gtkWidget().get_policy()
		return self.__policiesToModes[p[0]]
		
	def setVerticalMode( self, mode ) :
	
		p = self.gtkWidget().get_policy()
		self.gtkWidget().set_policy( p[0], self.__modesToPolicies[mode] )

	def getVerticalMode( self ) :
	
		p = self.gtkWidget().get_policy()
		return self.__policiesToModes[p[1]]
		
GafferUI.Widget._parseRCStyle(

	"""
	style "gafferScrollbar"
	{
		GtkScrollbar::slider-width = 12
		GtkScrollbar::has-backward-stepper = 0
		GtkScrollbar::has-secondary-backward-stepper = 1
		bg[ACTIVE] = $dull
		bg[PRELIGHT] = $bright
		bg[SELECTED] = $bright
	}

	widget_class "*<GtkScrollbar>*" style "gafferScrollbar"
	""",
	
	{
		"dull" : GafferUI.Widget._gtkRCColor( IECore.Color3f( 0.05 ) ),
		"bright" : GafferUI.Widget._gtkRCColor( IECore.Color3f( 0.1 ) ),
	}

)
		
