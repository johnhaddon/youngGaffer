import gtk
import gobject

## This class is manages the event loop used to run GafferUI based applications.
class EventLoop() :

	## Starts the event loop, passing control to the UI code. This will return when
	# EventLoop.stop() is called. This may be called recursively.
	@staticmethod
	def start() :
	
		gtk.main()
	
	## Stops the event loop last started using start().
	@staticmethod
	def stop() :
	
		gtk.main_quit()
	
	## Adds a function to be called when the event loop is idle (has no events
	# remaining to be processed). Returns an id which can be used with removeIdleCallback().
	# If callback returns False then it will be removed automatically after running,
	# if it returns True it will be called again until it returns False, or until
	# removeIdleCallback() is called.
	@staticmethod
	def addIdleCallback( callback ) :
		
		return gobject.idle_add( callback )
	
	## Removes an idle callback previously created with addIdleCallback().
	@staticmethod
	def removeIdleCallback( callbackId ) :
	
		gobject.source_remove( callbackId )
