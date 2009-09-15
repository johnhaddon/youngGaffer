class About :

	@staticmethod
	def name() :
	
		return "Gaffer"

	@staticmethod
	def majorVersion() :
	
		return !GAFFER_MAJOR_VERSION!

	@staticmethod
	def minorVersion() :
	
		return !GAFFER_MINOR_VERSION!
		
	@staticmethod
	def patchVersion() :
	
		return !GAFFER_PATCH_VERSION!
		
	@staticmethod
	def versionString() :
	
		return "%d.%d.%d" % ( About.majorVersion(), About.minorVersion(), About.patchVersion() )	

	@staticmethod
	def copyright() :
		
		return "Copyright (c) 2009 John Haddon"
		
	@staticmethod
	def url() :
	
		return "http://haddonindustries.ca"
	
	@staticmethod	
	def dependencies() :
	
		return [
		
			{
				"name" : "atk",
				"url" : "http://www.gtk.org/",
				"license" : "$GAFFER_ROOT/doc/licenses/atk",
				"download" : About.url() + "/downloads/opensource/atk-1.26.0.tar.gz",
			},
			
			{
				"name" : "boost",
				"url" : "http://www.boost.org/",
				"license" : "$GAFFER_ROOT/doc/licenses/boost",
			},

			{
				"name" : "cairo",
				"url" : "http://cairographics.org/",
				"license" : "$GAFFER_ROOT/doc/licenses/cairo",
				"download" : About.url() + "/downloads/opensource/cairo-1.8.6.tar.gz",
			},
			
			{
				"name" : "cortex",
				"url" : "http://code.google.com/p/cortex-vfx",
				"license" : "$GAFFER_ROOT/doc/licenses/cortex",
			},
			
			{
				"name" : "expat",
				"url" : "http://expat.sourceforge.net/",
				"license" : "$GAFFER_ROOT/doc/licenses/expat",
			},
			
			{
				"name" : "fontconfig",
				"url" : "http://www.fontconfig.org/",
				"license" : "$GAFFER_ROOT/doc/licenses/fontconfig",
			},
			
			{
				"name" : "freetype",
				"url" : "http://www.freetype.org/",
				"license" : "$GAFFER_ROOT/doc/licenses/freetype",
				"credit" : "Portions of this software are copyright (c) 2009 The FreeType Project (www.freetype.org). All rights reserved."
			},

			{
				"name" : "libintl",
				"url" : "http://www.gnu.org/software/gettext/",
				"license" : "$GAFFER_ROOT/doc/licenses/libintl",
			},
			
			{
				"name" : "glew",
				"url" : "http://glew.sourceforge.net/",
				"license" : "$GAFFER_ROOT/doc/licenses/glew",
			},
			
			{
				"name" : "gtk+",
				"url" : "http://www.gtk.org/",
				"license" : "$GAFFER_ROOT/doc/licenses/gtk+",
				"download" : About.url() + "/downloads/opensource/gtk+-2.16.1.tar.gz",
			},
			
			{
				"name" : "gtkglext",
				"url" : "http://gtkglext.sourceforge.net/",
				"license" : "$GAFFER_ROOT/doc/licenses/gtkglext",
				"download" : About.url() + "/downloads/opensource/gtkglext-1.2.0.tar.gz",
			},
			
			{
				"name" : "ilmbase",
				"url" : "http://www.openexr.com/",
				"license" : "$GAFFER_ROOT/doc/licenses/ilmbase",
			},
			
			{
				"name" : "libjpeg",
				"url" : "http://www.ijg.org/",
				"license" : "$GAFFER_ROOT/doc/licenses/libjpeg",
				"credit" : "This software is based in part on the work of the Independent JPEG Group.",
			},
			
			{
				"name" : "libpng",
				"url" : "http://www.libpng.org/pub/png/libpng.html",
				"license" : "$GAFFER_ROOT/doc/licenses/libpng",
			},
			
			{
				"name" : "openexr",
				"url" : "http://www.openexr.com/",
				"license" : "$GAFFER_ROOT/doc/licenses/openexr",
			},
			
			{
				"name" : "pango",
				"url" : "http://www.pango.org/",
				"license" : "$GAFFER_ROOT/doc/licenses/pango",
				"download" : About.url() + "/downloads/opensource/pango-1.24.1.tar.gz",
			},
			
			{
				"name" : "pixman",
				"url" : "http://cairographics.org/",
				"license" : "$GAFFER_ROOT/doc/licenses/pixman",
			},
			
			{
				"name" : "pycairo",
				"url" : "http://cairographics.org/",
				"license" : "$GAFFER_ROOT/doc/licenses/pycairo",
				"download" : About.url() + "/downloads/opensource/pycairo-1.8.4.tar.gz",
			},
			
			{
				"name" : "pygobject",
				"url" : "http://www.pygtk.org/",
				"license" : "$GAFFER_ROOT/doc/licenses/pygobject",
				"download" : About.url() + "/downloads/opensource/pygobject-2.16.1.tar.gz",
			},
			
			{
				"name" : "pygtk",
				"url" : "http://www.pygtk.org/",
				"license" : "$GAFFER_ROOT/doc/licenses/pygtk",
				"download" : About.url() + "/downloads/opensource/pygtk-2.14.1.tar.gz",
			},
			
			{
				"name" : "pygtkglext",
				"url" : "http://gtkglext.sourceforge.net/",
				"license" : "$GAFFER_ROOT/doc/licenses/pygtkglext",
				"download" : About.url() + "/downloads/opensource/pygtkglext-1.1.0.tar.gz",
			},
			
			{
				"name" : "python",
				"url" : "http://python.org/",
				"license" : "$GAFFER_ROOT/doc/licenses/python",
			},
			
			{
				"name" : "pyopengl",
				"url" : "http://pyopengl.sourceforge.net/",
			},
			
			{
				"name" : "libtiff",
				"url" : "http://www.libtiff.org/",
				"license" : "$GAFFER_ROOT/doc/licenses/libtiff",
			},
							
		]	
