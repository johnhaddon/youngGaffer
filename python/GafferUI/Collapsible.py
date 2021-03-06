import gtk

import IECore

import GafferUI

class Collapsible( GafferUI.ContainerWidget ) :

	def __init__( self, label="", child=None, collapsed=False ) :
	
		GafferUI.ContainerWidget.__init__( self, gtk.Expander() )
		
		labelWidget = gtk.Label()
		labelWidget.show()
		self.gtkWidget().set_label_widget( labelWidget )
		
		self.setLabel( label )
		self.setCollapsed( collapsed )
		
		self.__child = None
		self.setChild( child )
		
		self.__stateChangedSignal = GafferUI.WidgetSignal()
		self.gtkWidget().connect( "notify::expanded", self.__stateChanged )
		
	def removeChild( self, child ) :
	
		assert( child is self.__child )
		self.gtkWidget().remove( self.__child.gtkWidget() )
		self.__child = None
		
	def setChild( self, child ) :
	
		if self.__child :
			self.removeChild( self.__child )
		
		if child is not None :
			self.gtkWidget().add( child.gtkWidget() )
			self.__child = child	

	def getChild( self ) :
	
		return self.__child
	
	def setLabel( self, label ) :
				
		self.gtkWidget().get_label_widget().set_text( label )
		
	def getLabel( self ) :
	
		return self.gtkWidget().get_label_widget().get_text()
	
	def setCollapsed( self, state ) :
	
		self.gtkWidget().set_expanded( not state )
				
	def getCollapsed( self ) :
	
		return not self.gtkWidget().get_expanded()
	
	## A signal emitted whenever the ui is collapsed or
	# expanded.
	def stateChangedSignal( self ) :
	
		return self.__stateChangedSignal
		
	def __stateChanged( self, widget, unused ) :
	
		assert( widget is self.gtkWidget() )
		
		self.stateChangedSignal()( self )
		
GafferUI.Widget._parseRCStyle(

	"""
	style "gafferCollapsible"
	{
		bg[ACTIVE] = $bg
		bg[PRELIGHT] = $bg
		bg[SELECTED] = $bg
		fg[ACTIVE] = $fg
		fg[PRELIGHT] = $fg
		fg[SELECTED] = $fg
	}

	widget_class "*<GtkExpander>" style "gafferCollapsible"
	""",
	
	{
		"bg" : GafferUI.Widget._gtkRCColor( IECore.Color3f( 0.1 ) ),
		"fg" : GafferUI.Widget._gtkRCColor( IECore.Color3f( 0.8 ) ),
	}

)
