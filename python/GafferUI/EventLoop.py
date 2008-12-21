import gtk

## This class is manages the event loop used to run GafferUI based applications.
class EventLoop() :

	## Starts the event loop, passing control to the UI code. This will return when
	# EventLoop.stop() is called. This may be called recursively.
	@staticmethod
	def start() :
	
		gtk.main()
	
	@staticmethod
	def stop() :
	
		gtk.main_quit()
	
