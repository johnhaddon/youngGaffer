import os

def showURL( url ) :

	# it'd be nice to use gtk.show_uri but i can't seem
	# to get that to work
	os.system( "open %s" % url )
