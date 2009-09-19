import os
import sys
import glob
import shutil
import py_compile

CacheDir( "/Users/john/dev/sconsBuildCache" )

###############################################################################################
# Command line options
###############################################################################################

options = Variables( "", ARGUMENTS )

options.Add(
	"BUILD_DIR",
	"The destination directory in which the build will be made.",
	"/Users/john/dev/build/gaffer"
)

options.Add(
	"INSTALL_DIR",
	"The destination directory for the installation.",
	"/Users/john/dev/install/gaffer",
)

options.Add(
	"PACKAGE_FILE",
	"The file in which the final gaffer file will be created by the package target.",
	"/Users/john/dev/install/gaffer.tar.gz",
)

options.Add( 
	BoolVariable( "BUILD_DEPENDENCIES", "Set this to build all the library dependencies gaffer has.", False )
)

options.Add(
	"DEPENDENCIES_SRC_DIR",
	"The location of a directory holding dependencies.",
	"/Users/john/dev/gafferDependencies",
)

options.Add(
	BoolVariable( "BUILD_PYTHON", "Set this to build python.", "$BUILD_DEPENDENCIES" )
)

options.Add(
	"PYTHON_SRC_DIR",
	"The location of the python source to be used if BUILD_PYTHON is specified.",
	"$DEPENDENCIES_SRC_DIR/Python-2.6.1",
)

options.Add(
	BoolVariable( "BUILD_BOOST", "Set this to build boost.", "$BUILD_DEPENDENCIES" )
)

options.Add(
	"BOOST_SRC_DIR",
	"The location of the boost source to be used if BUILD_BOOST is specified.",
	"$DEPENDENCIES_SRC_DIR/boost_1_38_0",
)

options.Add(
	BoolVariable( "BUILD_OPENEXR", "Set this to build openexr.", "$BUILD_DEPENDENCIES" )
)

options.Add(
	"ILMBASE_SRC_DIR",
	"The location of the ilmbase source to be used if BUILD_OPENEXR is specified.",
	"$DEPENDENCIES_SRC_DIR/ilmbase-1.0.1",
)

options.Add(
	"OPENEXR_SRC_DIR",
	"The location of the exr source to be used if BUILD_OPENEXR is specified.",
	"$DEPENDENCIES_SRC_DIR/openexr-1.6.1",
)

options.Add(
	BoolVariable( "BUILD_JPEG", "Set this to build the jpeg library.", "$BUILD_DEPENDENCIES" )
)

options.Add(
	"JPEG_SRC_DIR",
	"The location of the jpeg source to be used if BUILD_JPEG is specified.",
	"$DEPENDENCIES_SRC_DIR/jpeg-6b",
)

options.Add(
	BoolVariable( "BUILD_TIFF", "Set this to build the tiff library.", "$BUILD_DEPENDENCIES" )
)

options.Add(
	"TIFF_SRC_DIR",
	"The location of the tiff source to be used if BUILD_TIFF is specified.",
	"$DEPENDENCIES_SRC_DIR/tiff-3.8.2",
)

options.Add(
	BoolVariable( "BUILD_FREETYPE", "Set this to build freetype.", "$BUILD_DEPENDENCIES" )
)

options.Add(
	"FREETYPE_SRC_DIR",
	"The location of the freetype source to be used if BUILD_FREETYPE is specified.",
	"$DEPENDENCIES_SRC_DIR/freetype-2.3.9",
)

options.Add(
	BoolVariable( "BUILD_GLEW", "Set this to build GLEW.", "$BUILD_DEPENDENCIES" )
)

options.Add(
	"GLEW_SRC_DIR",
	"The location of the freetype source to be used if BUILD_GLEW is specified.",
	"$DEPENDENCIES_SRC_DIR/glew",
)

options.Add(
	BoolVariable( "BUILD_CORTEX", "Set this to build cortex.", "$BUILD_DEPENDENCIES" )
)

options.Add(
	"CORTEX_SRC_DIR",
	"The location of the boost source to be used if BUILD_CORTEX is specified.",
	"$DEPENDENCIES_SRC_DIR/cortex/trunk",
)

options.Add(
	BoolVariable( "BUILD_GTK", "Set this to build gtk.", "$BUILD_DEPENDENCIES" )
)

options.Add(
	"PKGCONFIG_SRC_DIR",
	"The location of the pkg-config source to be used if BUILD_GTK is specified.",
	"$DEPENDENCIES_SRC_DIR/pkg-config-0.23",
)

options.Add(
	"GLIB_SRC_DIR",
	"The location of the glib source to be used if BUILD_GTK is specified.",
	"$DEPENDENCIES_SRC_DIR/glib-2.20.1",
)

options.Add(
	"ATK_SRC_DIR",
	"The location of the atk source to be used if BUILD_GTK is specified.",
	"$DEPENDENCIES_SRC_DIR/atk-1.26.0",
)

options.Add(
	"PNG_SRC_DIR",
	"The location of the libpng source to be used if BUILD_GTK is specified.",
	"$DEPENDENCIES_SRC_DIR/libpng-1.2.35",
)

options.Add(
	"EXPAT_SRC_DIR",
	"The location of the expat source to be used if BUILD_GTK is specified.",
	"$DEPENDENCIES_SRC_DIR/expat-2.0.1",
)

options.Add(
	"FONTCONFIG_SRC_DIR",
	"The location of the fontconfig source to be used if BUILD_GTK is specified.",
	"$DEPENDENCIES_SRC_DIR/fontconfig-2.6.0",
)

options.Add(
	"CAIRO_SRC_DIR",
	"The location of the cairo source to be used if BUILD_GTK is specified.",
	"$DEPENDENCIES_SRC_DIR/cairo-1.8.6",
)

options.Add(
	"PIXMAN_SRC_DIR",
	"The location of the pixman source to be used if BUILD_GTK is specified.",
	"$DEPENDENCIES_SRC_DIR/pixman-0.15.2",
)

options.Add(
	"PANGO_SRC_DIR",
	"The location of the pango source to be used if BUILD_GTK is specified.",
	"$DEPENDENCIES_SRC_DIR/pango-1.24.1",
)

options.Add(
	"GETTEXT_SRC_DIR",
	"The location of the gettext source to be used if BUILD_GTK is specified.",
	"$DEPENDENCIES_SRC_DIR/gettext-0.17",
)

options.Add(
	"GTK_SRC_DIR",
	"The location of the gtk source to be used if BUILD_GTK is specified.",
	"$DEPENDENCIES_SRC_DIR/gtk+-2.16.1",
)

options.Add(
	"PYGOBJECT_SRC_DIR",
	"The location of the pygobject source to be used if BUILD_GTK is specified.",
	"$DEPENDENCIES_SRC_DIR/pygobject-2.16.1",
)

options.Add(
	"PYCAIRO_SRC_DIR",
	"The location of the pycairo source to be used if BUILD_GTK is specified.",
	"$DEPENDENCIES_SRC_DIR/pycairo-1.8.4",
)

options.Add(
	"PYGTK_SRC_DIR",
	"The location of the pytgtk source to be used if BUILD_GTK is specified.",
	"$DEPENDENCIES_SRC_DIR/pygtk-2.14.1",
)

options.Add(
	"GTKGLEXT_SRC_DIR",
	"The location of the gtkglext source to be used if BUILD_GTK is specified.",
	"$DEPENDENCIES_SRC_DIR/gtkglext-1.2.0",
)

options.Add(
	"PYGTKGLEXT_SRC_DIR",
	"The location of the pygtkglext source to be used if BUILD_GTK is specified.",
	"$DEPENDENCIES_SRC_DIR/pygtkglext-1.1.0",
)

options.Add(
	"PYOPENGL_SRC_DIR",
	"The location of the pygtkglext source to be used if BUILD_GTK is specified.",
	"$DEPENDENCIES_SRC_DIR/PyOpenGL-3.0.0",
)

###############################################################################################
# Dependencies
# They doesn't fit into the SCons way of things too well so we just build them directly when
# the script runs.
###############################################################################################

depEnv = Environment(
	options = options,
)

def runCommand( command ) :

	sys.stderr.write( command + "\n" )
	command = "export PATH=%s DYLD_LIBRARY_PATH= && %s " % ( depEnv.subst( "$BUILD_DIR/bin:/usr/local/bin:/bin:/sbin:/usr/bin:/usr/sbin" ), depEnv.subst( command ) )
	status = os.system( command )
	if status :
		raise RuntimeError( "Failed to build dependency" )

pythonBuild = None
if depEnv["BUILD_PYTHON"] :
	runCommand( "cd $PYTHON_SRC_DIR; ./configure --enable-framework=$BUILD_DIR/frameworks --prefix=$BUILD_DIR && make clean && make && make install" )
	runCommand( "cd $BUILD_DIR/bin && ln -fsh python2.6 python" )
	
if depEnv["BUILD_BOOST"] :
	runCommand( "cd $BOOST_SRC_DIR; ./configure --prefix=$BUILD_DIR --with-python=$BUILD_DIR/bin/python2.6 && make clean && make && make install" )

if depEnv["BUILD_OPENEXR"] :
	runCommand( "cd $ILMBASE_SRC_DIR && ./configure --prefix=$BUILD_DIR && make && make install" )
	runCommand( "cd $OPENEXR_SRC_DIR && ./configure --prefix=$BUILD_DIR && make && make install" )

if depEnv["BUILD_JPEG"] :
	runCommand( "cd $JPEG_SRC_DIR && ./configure --prefix=$BUILD_DIR && make clean && make && make install-lib install-headers" )

if depEnv["BUILD_TIFF"] :
	runCommand( "cd $TIFF_SRC_DIR && ./configure --prefix=$BUILD_DIR && make && make install" )
		
if depEnv["BUILD_FREETYPE"] :
	runCommand( "cd $FREETYPE_SRC_DIR && ./configure --prefix=$BUILD_DIR && make && make install" )

if depEnv["BUILD_GLEW"] :
	runCommand( "cd $GLEW_SRC_DIR && make install GLEW_DEST=$BUILD_DIR" )
	
if depEnv["BUILD_CORTEX"] :
	runCommand( "cd $CORTEX_SRC_DIR; /usr/local/bin/python /opt/local/bin/scons -j 2 install INSTALL_PREFIX=$BUILD_DIR INSTALL_PYTHON_DIR=$BUILD_DIR/lib/python2.6/site-packages PYTHON_CONFIG=$BUILD_DIR/bin/python2.6-config BOOST_INCLUDE_PATH=$BUILD_DIR/include/boost-1_38 LIBPATH=$BUILD_DIR/lib BOOST_LIB_SUFFIX=-xgcc40-mt-1_38 OPENEXR_INCLUDE_PATH=$BUILD_DIR/include FREETYPE_INCLUDE_PATH=$BUILD_DIR/include/freetype2 RMAN_ROOT=/Applications/Graphics/3Delight-8.5.9 WITH_GL=1 GLEW_INCLUDE_PATH=$BUILD_DIR/include/GL DOXYGEN=/opt/local/bin/doxygen INSTALL_DOC_DIR=$BUILD_DIR/doc/cortex" )
	
if depEnv["BUILD_GTK"] :
	runCommand( "cd $GETTEXT_SRC_DIR && ./configure --prefix=$BUILD_DIR && make && make install" )
	runCommand( "cd $PKGCONFIG_SRC_DIR && ./configure --prefix=$BUILD_DIR && make && make install" )
	runCommand( "echo '#!/bin/sh\n' > $BUILD_DIR/bin/gtkdoc-rebase && chmod +x $BUILD_DIR/bin/gtkdoc-rebase" ) # hack to provide a stub script for glib to think it is building documentation (even though it was told not to)
	runCommand( "cd $GLIB_SRC_DIR && ./configure --prefix=$BUILD_DIR CPPFLAGS=-I$BUILD_DIR/include LDFLAGS=-L$BUILD_DIR/lib && make && make install" )
	runCommand( "cd $ATK_SRC_DIR && ./configure --prefix=$BUILD_DIR && make && make install" )
	runCommand( "cd $PIXMAN_SRC_DIR && ./configure --prefix=$BUILD_DIR && make && make install" )
	runCommand( "cd $PNG_SRC_DIR && ./configure --prefix=$BUILD_DIR && make && make install" )
	runCommand( "cd $EXPAT_SRC_DIR && ./configure --prefix=$BUILD_DIR && make && make install" )
	runCommand( "cd $FONTCONFIG_SRC_DIR && ./configure --prefix=$BUILD_DIR CPPFLAGS=-I$BUILD_DIR/include LDFLAGS=-L$BUILD_DIR/lib && make && make install" )
	runCommand( "cd $CAIRO_SRC_DIR && ./configure --prefix=$BUILD_DIR PKG_CONFIG=$BUILD_DIR/bin/pkg-config && make && make install" )
	runCommand( "cd $PANGO_SRC_DIR && ./configure --with-included-modules=yes --prefix=$BUILD_DIR PKG_CONFIG=$BUILD_DIR/bin/pkg-config && make clean && make && make install" )	
	runCommand( "cd $GTK_SRC_DIR && ./configure --prefix=$BUILD_DIR CPPFLAGS=-I$BUILD_DIR/include LDFLAGS=-L$BUILD_DIR/lib PKG_CONFIG=$BUILD_DIR/bin/pkg-config --without-libjasper && make && make install" )
	runCommand( "cd $PYGOBJECT_SRC_DIR && ./configure --prefix=$BUILD_DIR CPPFLAGS=-I$BUILD_DIR/include LDFLAGS=-L$BUILD_DIR/lib PKG_CONFIG=$BUILD_DIR/bin/pkg-config && make && make install" )
	runCommand( "cd $PYCAIRO_SRC_DIR && ./configure --prefix=$BUILD_DIR CPPFLAGS=-I$BUILD_DIR/include LDFLAGS=-L$BUILD_DIR/lib PKG_CONFIG=$BUILD_DIR/bin/pkg-config && make clean && make && make install" )
	runCommand( "cd $PYGTK_SRC_DIR && ./configure --prefix=$BUILD_DIR CPPFLAGS=-I$BUILD_DIR/include LDFLAGS=-L$BUILD_DIR/lib PKG_CONFIG=$BUILD_DIR/bin/pkg-config && make clean && make && make install" )
	runCommand( "cd $GTKGLEXT_SRC_DIR && ./configure --prefix=$BUILD_DIR CPPFLAGS=-I$BUILD_DIR/include LDFLAGS=-L$BUILD_DIR/lib PKG_CONFIG=$BUILD_DIR/bin/pkg-config --with-gl-includedir=/usr/X11R6/include && make clean && make && make install" )
	runCommand( "cd $PYGTKGLEXT_SRC_DIR && ./configure --prefix=$BUILD_DIR CPPFLAGS=-I$BUILD_DIR/include LDFLAGS=-L$BUILD_DIR/lib PKG_CONFIG=$BUILD_DIR/bin/pkg-config && make clean && make && make install" )
	runCommand( "cd $PYOPENGL_SRC_DIR && python setup.py install" )
	
###############################################################################################
# Gaffer libraries
###############################################################################################

boostLibSuffix = "-xgcc40-mt-1_38"

env = Environment(

	options = options,

	GAFFER_MAJOR_VERSION = "0",
	GAFFER_MINOR_VERSION = "1",
	GAFFER_PATCH_VERSION = "0",

	CPPPATH = [
		"include",
		"$BUILD_DIR/include",
		"$BUILD_DIR/include/boost-1_38",
		"$BUILD_DIR/include/OpenEXR",
	],
	
	CXXFLAGS = [
		"-Wall",
		"-Werror",
		"-O2",
	],
	
	LIBPATH = [
		"$BUILD_DIR/lib"
	],
	
	LIBS = [
		"boost_signals" + boostLibSuffix,
		"boost_iostreams" + boostLibSuffix,
		"boost_filesystem" + boostLibSuffix,
		"boost_date_time" + boostLibSuffix,
		"boost_thread" + boostLibSuffix,
		"boost_wave" + boostLibSuffix,
		"boost_regex" + boostLibSuffix,		
		"boost_system" + boostLibSuffix,
		"Imath",
		"IECore",
	],
	
)

gafferLibrary = env.SharedLibrary( "lib/Gaffer", glob.glob( "src/Gaffer/*.cpp" ) )
env.Default( gafferLibrary )

gafferLibraryInstall = env.Install( "$BUILD_DIR/lib", gafferLibrary )
env.Alias( "build", gafferLibraryInstall )

uiEnv = env.Clone()
uiEnv.Append(

	LIBS = [ gafferLibrary ],

)
gafferUILibrary = uiEnv.SharedLibrary( "lib/GafferUI", glob.glob( "src/GafferUI/*.cpp" ) )
uiEnv.Default( gafferUILibrary )

gafferUILibraryInstall = env.Install( "$BUILD_DIR/lib", gafferUILibrary )
env.Alias( "build", gafferUILibraryInstall )

###############################################################################################
# Gaffer python modules
###############################################################################################

pythonEnv = env.Clone()

pythonEnv.Append(

	CPPFLAGS = [
		"-DBOOST_PYTHON_MAX_ARITY=20",
	] + os.popen( pythonEnv.subst( "$BUILD_DIR/bin/python2.6-config --includes" ) ).read().split(),
	
	LIBPATH = [ "./lib" ],
	
	LIBS = [
		"boost_python" + boostLibSuffix,
		"Gaffer",
	],
	
	SHLINKFLAGS = os.popen( pythonEnv.subst( "$BUILD_DIR/bin/python2.6-config --ldflags" ) ).read().split(),
)

if pythonEnv["PLATFORM"]=="darwin" :
	pythonEnv.Append( SHLINKFLAGS = "-single_module" )

gafferBindingsLibrary = pythonEnv.SharedLibrary( "lib/GafferBindings", glob.glob( "src/GafferBindings/*.cpp" ) )
pythonEnv.Default( gafferBindingsLibrary )

gafferBindingsLibraryInstall = env.Install( "$BUILD_DIR/lib", gafferBindingsLibrary )
env.Alias( "build", gafferBindingsLibraryInstall )

pythonUIEnv = pythonEnv.Clone()
pythonUIEnv.Append(
	LIBS = [ "GafferUI", "GafferBindings" ],
)
gafferUIBindingsLibrary = pythonUIEnv.SharedLibrary( "lib/GafferUIBindings", glob.glob( "src/GafferUIBindings/*.cpp" ) )
pythonUIEnv.Default( gafferUIBindingsLibrary )

gafferUIBindingsLibraryInstall = env.Install( "$BUILD_DIR/lib", gafferUIBindingsLibrary )
env.Alias( "build", gafferUIBindingsLibraryInstall )

pythonModuleEnv = pythonEnv.Clone()
pythonModuleEnv.Append(

	LIBS = [
		"GafferBindings",
	]

)

pythonModuleEnv["SHLIBPREFIX"] = ""
pythonModuleEnv["SHLIBSUFFIX"] = ".so"

gafferModule = pythonModuleEnv.SharedLibrary( "python/Gaffer/_Gaffer", glob.glob( "src/GafferModule/*.cpp" ) )
pythonModuleEnv.Default( gafferModule )

gafferModuleInstall = env.Install( "$BUILD_DIR/lib/python2.6/site-packages/Gaffer", gafferModule )
sedSubstitutions = "s/!GAFFER_MAJOR_VERSION!/$GAFFER_MAJOR_VERSION/g"
sedSubstitutions += "; s/!GAFFER_MINOR_VERSION!/$GAFFER_MINOR_VERSION/g"
sedSubstitutions += "; s/!GAFFER_PATCH_VERSION!/$GAFFER_PATCH_VERSION/g"

for f in glob.glob( "python/Gaffer/*.py" ) :
	gafferModuleInstall += env.Command( "$BUILD_DIR/lib/python2.6/site-packages/Gaffer/" + os.path.basename( f ), f, "sed \"" + sedSubstitutions + "\" $SOURCE > $TARGET" )

env.Alias( "build", gafferModuleInstall )

pythonUIModuleEnv = pythonModuleEnv.Clone()
pythonUIModuleEnv.Append(
	LIBS = [ "GafferUI", "GafferUIBindings" ],
)
gafferUIModule = pythonUIModuleEnv.SharedLibrary( "python/GafferUI/_GafferUI", glob.glob( "src/GafferUIModule/*.cpp" ) )
pythonUIModuleEnv.Default( gafferUIModule )

gafferUIModuleInstall = env.Install( "$BUILD_DIR/lib/python2.6/site-packages/GafferUI", gafferUIModule )
gafferUIModuleInstall += env.Install( "$BUILD_DIR/lib/python2.6/site-packages/GafferUI", glob.glob( "python/GafferUI/*.py" ) )
env.Alias( "build", gafferUIModuleInstall )

for module in ( "GafferTest", "GafferUITest" ) :

	moduleInstall = env.Install( "$BUILD_DIR/lib/python2.6/site-packages/" + module, glob.glob( "python/%s/*.py" % module ) )
	env.Alias( "build", moduleInstall ) 

###############################################################################################
# Scripts and apps and stuff
###############################################################################################

scriptsInstall = env.Install( "$BUILD_DIR/bin", [ "bin/gaffer", "bin/gaffer.py" ] )
env.Alias( "build", scriptsInstall )

for app in ( "gui", "view", "test", "cli", "license" ) :
	appInstall = env.Install( "$BUILD_DIR/apps/%s/1" % app, "apps/%s/1/%s.py" % ( app, app ) )
	env.Alias( "build", appInstall )

startupScriptsInstall = env.Install( "$BUILD_DIR/startup/ui", glob.glob( "startup/ui/*.py" ) )
env.Alias( "build", startupScriptsInstall )

for d in ( "ui", "" ) :
	shaderInstall = env.Install( "$BUILD_DIR/shaders/%s/" % d, glob.glob( "shaders/%s/*" % d ) )
	env.Alias( "build", shaderInstall )
	
#########################################################################################################
# Documentation
#########################################################################################################

def docMunger( target, source, env ) :

	f = open( str( source[0] ) )
	o = open( str( target[0] ), "w" )
	for l in f :
		
		w = l.split()
		if len( w ) < 2 :
			continue
			
		if w[0]=="from" :
			
			if not w[1].startswith( "_" ) :
				ff = open( os.path.dirname( str( source[0] ) ) + "/" + w[1] + ".py" )
				for ll in ff :
					o.write( ll )
					
		elif w[0]=="import" :
			sourceFileName = os.path.dirname( str( source[0] ) ) + "/" + w[1] + ".py"
			if os.path.exists( sourceFileName ) :
				shutil.copyfile( sourceFileName, os.path.dirname( str( target[0] ) ) + "/" + w[1] + ".py" )
	
docEnv = env.Clone()
docEnv["ENV"]["PATH"] = os.environ["PATH"]
for v in ( "BUILD_DIR", "GAFFER_MAJOR_VERSION", "GAFFER_MINOR_VERSION", "GAFFER_PATCH_VERSION" ) :
	docEnv["ENV"][v] = docEnv[v]

gafferMunged = env.Command( "doc/src/Gaffer.py", "python/Gaffer/__init__.py", docMunger )
env.Depends( gafferMunged, glob.glob( "python/Gaffer/*.py" ) )

gafferUIMunged = env.Command( "doc/src/GafferUI.py", "python/GafferUI/__init__.py", docMunger )
env.Depends( gafferUIMunged, glob.glob( "python/GafferUI/*.py" ) )

docs = docEnv.Command( "doc/html/index.html", "doc/config/Doxyfile", "doxygen doc/config/Doxyfile" )
env.NoCache( docs )
docEnv.Depends( docs, glob.glob( "include/*/*.h" ) + gafferMunged + gafferUIMunged + glob.glob( "doc/src/*.dox" ) )

docInstall = docEnv.Install( "$BUILD_DIR/doc/gaffer", "doc/html" )
docEnv.Alias( "build", docInstall )

#########################################################################################################
# Installation
#########################################################################################################

manifest = [
	"bin/gaffer",
	"bin/gaffer.py",
	"apps/cli/1/cli.py",
	"apps/gui/1/gui.py",
	"apps/test/1/test.py",
	"apps/view/1/view.py",
	"apps/license/1/license.py",
	"lib/libboost_signals" + boostLibSuffix + ".dylib",
	"lib/libboost_thread" + boostLibSuffix + ".dylib",
	"lib/libboost_wave" + boostLibSuffix + ".dylib",
	"lib/libboost_regex" + boostLibSuffix + ".dylib",
	"lib/libboost_python" + boostLibSuffix + ".dylib",
	"lib/libboost_date_time" + boostLibSuffix + ".dylib",
	"lib/libboost_filesystem" + boostLibSuffix + ".dylib",
	"lib/libboost_iostreams" + boostLibSuffix + ".dylib",
	"lib/libboost_system" + boostLibSuffix + ".dylib",
	"lib/libIECore.dylib",
	"lib/libIECoreGL.dylib",
	"lib/libIECoreRI.dylib",
	"lib/libGaffer.dylib",
	"lib/libGafferBindings.dylib",
	"lib/libGafferUI.dylib",
	"lib/libGafferUIBindings.dylib",
	"lib/libIex.6.dylib",
	"lib/libHalf.6.dylib",
	"lib/libImath.6.dylib",
	"lib/libIlmImf.6.dylib",
	"lib/libIlmThread.6.dylib",
	"lib/libtiff.3.dylib",
	"lib/libfreetype.6.dylib",
	"lib/libpyglib-2.0.0.dylib",
	"lib/libgobject-2.0.0.dylib",
	"lib/libgthread-2.0.0.dylib",
	"lib/libglib-2.0.0.dylib",
	"lib/libintl.8.dylib",
	"lib/libgtk-x11-2.0.0.dylib",
	"lib/libgdk-x11-2.0.0.dylib",
	"lib/libatk-1.0.0.dylib",
	"lib/libgdk_pixbuf-2.0.0.dylib",
	"lib/libgio-2.0.0.dylib",
	"lib/libpangocairo-1.0.0.dylib",
	"lib/libpangoft2-1.0.0.dylib",
	"lib/libcairo.2.dylib",
	"lib/libpixman-1.0.dylib",
	"lib/libpng12.0.dylib",
	"lib/libpango-1.0.0.dylib",
	"lib/libfontconfig.1.dylib",
	"lib/libexpat.1.dylib",
	"lib/libgmodule-2.0.0.dylib",
	"lib/libgtkglext-x11-1.0.0.dylib",
	"lib/libgdkglext-x11-1.0.0.dylib",
	"lib/libpangox-1.0.0.dylib",
	"lib/libGLEW.1.5.0.dylib",
	"frameworks/Python.framework",
	"startup/ui/menus.py",
	"startup/ui/layouts.py",
	"shaders",
	"glsl/IECoreGL",
	"doc/gaffer/html",
	"doc/cortex/html",
	"lib/python2.6/site-packages/IECore",
	"lib/python2.6/site-packages/IECoreGL",
	"lib/python2.6/site-packages/IECoreRI",
	"lib/python2.6/site-packages/Gaffer/_Gaffer.so",	
	"lib/python2.6/site-packages/GafferTest",	
	"lib/python2.6/site-packages/GafferUI/_GafferUI.so",
	"lib/python2.6/site-packages/GafferUITest",
	"lib/python2.6/site-packages/pygtk.pyc",
	"lib/python2.6/site-packages/gtk-2.0",
	"lib/python2.6/site-packages/cairo",
]

symlinks = [
	# have to symlink python on the mac as the bin/python you get otherwise is just a stub with a hardcoded full
	# path to wherever the framework was built
	( "bin/python", "../frameworks/Python.framework/Versions/2.6/Resources/Python.app/Contents/MacOS/Python" ),
]

pythonSourceToCompile = [
	"lib/python2.6/site-packages/Gaffer/*.py",
	"lib/python2.6/site-packages/GafferUI/*.py",
]

licenses = [
	( "python", "$PYTHON_SRC_DIR/LICENSE" ),
	( "atk", "$ATK_SRC_DIR/COPYING" ),
	( "boost", "$BOOST_SRC_DIR/LICENSE_1_0.txt" ),
	( "cairo", "$CAIRO_SRC_DIR/COPYING-LGPL-2.1" ),
	( "cortex", "$CORTEX_SRC_DIR/LICENSE" ),
	( "expat", "$EXPAT_SRC_DIR/COPYING" ),
	( "fontconfig", "$FONTCONFIG_SRC_DIR/COPYING" ),
	( "freetype", "$FREETYPE_SRC_DIR/docs/FTL.TXT" ),
	( "libintl", "$GETTEXT_SRC_DIR/gettext-runtime/intl/COPYING.LIB-2.1" ),
	( "glew", "$GLEW_SRC_DIR/LICENSE.txt" ),
	( "glib", "$GLIB_SRC_DIR/COPYING" ),
	( "gtk+", "$GTK_SRC_DIR/COPYING" ),
	( "gtkglext", "$GTKGLEXT_SRC_DIR/COPYING.LIB" ),
	( "ilmbase", "$ILMBASE_SRC_DIR/COPYING" ),
	( "libjpeg", "$JPEG_SRC_DIR/README" ),
	( "libpng", "$PNG_SRC_DIR/LICENSE" ),
	( "openexr", "$OPENEXR_SRC_DIR/LICENSE" ),
	( "pango", "$PANGO_SRC_DIR/COPYING" ),
	( "pixman", "$PIXMAN_SRC_DIR/COPYING" ),
	( "pycairo", "$PYCAIRO_SRC_DIR/COPYING" ),
	( "pygobject", "$PYGOBJECT_SRC_DIR/COPYING" ),
	( "pygtk", "$PYGTK_SRC_DIR/COPYING" ),
	( "pygtkglext", "$PYGTKGLEXT_SRC_DIR/COPYING" ),
	( "libtiff", "$TIFF_SRC_DIR/COPYRIGHT" ),
]

def expandSourceFiles( files, env ) :

	result = []
	root = env.subst( "$BUILD_DIR" )
	for f in files :
		for ff in glob.glob( os.path.join( root, f ) ) :
			result.append( ff[len(root)+1:] )
	
	return result

def installer( target, source, env ) :

	shutil.rmtree( env.subst( "$INSTALL_DIR" ), True )
	for f in manifest :

		src = env.subst( os.path.join( "$BUILD_DIR", f ) )
		dst = env.subst( os.path.join( "$INSTALL_DIR", f ) )
		dstDir = os.path.dirname( dst )
		if not os.path.isdir( dstDir ) :
			os.makedirs( dstDir )
				
		if os.path.isdir( src ) :	
			shutil.copytree( src, dst, True )
		else :
			shutil.copy( src, dst )
			
	for s in symlinks :
	
		os.symlink( s[1], env.subst( os.path.join( "$INSTALL_DIR", s[0] ) ) )

	for f in expandSourceFiles( pythonSourceToCompile, env ) :
		
		src = env.subst( os.path.join( "$BUILD_DIR", f ) )
		dst = env.subst( os.path.join( "$INSTALL_DIR", f + "c" ) )
		
		py_compile.compile( src, dst, doraise=True )
		
	licenseDir = env.subst( "$INSTALL_DIR/doc/licenses" )
	os.makedirs( licenseDir )
	for l in licenses :
	
		src = env.subst( l[1] )
		dst = os.path.join( licenseDir, l[0] )
		
		shutil.copy( src, dst )
										
install = env.Command( "$INSTALL_DIR/bin/gaffer", "$BUILD_DIR", installer )
env.AlwaysBuild( install )
env.NoCache( install )

env.Alias( "install", install )

#########################################################################################################
# Packaging
#########################################################################################################

def packager( target, source, env ) :

	installDir = env.subst( "$INSTALL_DIR" )
	b = os.path.basename( installDir )
	d = os.path.dirname( installDir )
	runCommand( env.subst( "tar -czf $PACKAGE_FILE -C %s %s" % ( d, b ) ) )
	
package = env.Command( "$PACKAGE_FILE", install, packager )
env.NoCache( package )
env.Alias( "package", package )
