import os
import sys
import glob

CacheDir( "/Users/john/dev/sconsBuildCache" )

###############################################################################################
# Command line options
###############################################################################################

options = Options( "", ARGUMENTS )

options.Add(
	"INSTALL_DIR",
	"The destination directory for the installation.",
	"/Users/john/dev/build/gaffer",
)

options.Add(
	"DST_DIR",
	"The destination directory in which the build will be made.",
	"/Users/john/dev/build/gaffer"
)

options.Add( 
	BoolOption( "BUILD_DEPENDENCIES", "Set this to build all the library dependencies gaffer has.", False )
)

options.Add(
	"DEPENDENCIES_SRC_DIR",
	"The location of a directory holding dependencies.",
	"/Users/john/dev/gafferDependencies",
)

options.Add(
	BoolOption( "BUILD_PYTHON", "Set this to build python.", "$BUILD_DEPENDENCIES" )
)

options.Add(
	"PYTHON_SRC_DIR",
	"The location of the python source to be used if BUILD_PYTHON is specified.",
	"$DEPENDENCIES_SRC_DIR/Python-2.6.1",
)

options.Add(
	BoolOption( "BUILD_BOOST", "Set this to build boost.", "$BUILD_DEPENDENCIES" )
)

options.Add(
	"BOOST_SRC_DIR",
	"The location of the boost source to be used if BUILD_BOOST is specified.",
	"$DEPENDENCIES_SRC_DIR/boost_1_38_0",
)

options.Add(
	BoolOption( "BUILD_OPENEXR", "Set this to build openexr.", "$BUILD_DEPENDENCIES" )
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
	BoolOption( "BUILD_JPEG", "Set this to build the jpeg library.", "$BUILD_DEPENDENCIES" )
)

options.Add(
	"JPEG_SRC_DIR",
	"The location of the jpeg source to be used if BUILD_JPEG is specified.",
	"$DEPENDENCIES_SRC_DIR/jpeg-6b",
)

options.Add(
	BoolOption( "BUILD_TIFF", "Set this to build the tiff library.", "$BUILD_DEPENDENCIES" )
)

options.Add(
	"TIFF_SRC_DIR",
	"The location of the tiff source to be used if BUILD_TIFF is specified.",
	"$DEPENDENCIES_SRC_DIR/tiff-3.8.2",
)

options.Add(
	BoolOption( "BUILD_FREETYPE", "Set this to build freetype.", "$BUILD_DEPENDENCIES" )
)

options.Add(
	"FREETYPE_SRC_DIR",
	"The location of the freetype source to be used if BUILD_FREETYPE is specified.",
	"$DEPENDENCIES_SRC_DIR/freetype-2.3.9",
)

options.Add(
	BoolOption( "BUILD_GLEW", "Set this to build GLEW.", "$BUILD_DEPENDENCIES" )
)

options.Add(
	"GLEW_SRC_DIR",
	"The location of the freetype source to be used if BUILD_GLEW is specified.",
	"$DEPENDENCIES_SRC_DIR/glew",
)

options.Add(
	BoolOption( "BUILD_CORTEX", "Set this to build cortex.", "$BUILD_DEPENDENCIES" )
)

options.Add(
	"CORTEX_SRC_DIR",
	"The location of the boost source to be used if BUILD_CORTEX is specified.",
	"$DEPENDENCIES_SRC_DIR/cortex/trunk",
)

options.Add(
	BoolOption( "BUILD_GTK", "Set this to build gtk.", "$BUILD_DEPENDENCIES" )
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
	command = "export PATH=%s DYLD_LIBRARY_PATH= && %s " % ( depEnv.subst( "$DST_DIR/bin:/usr/local/bin:/bin:/sbin:/usr/bin:/usr/sbin" ), depEnv.subst( command ) )
	status = os.system( command )
	if status :
		raise RuntimeError( "Failed to build dependency" )

pythonBuild = None
if depEnv["BUILD_PYTHON"] :
	runCommand( "cd $PYTHON_SRC_DIR; ./configure --enable-framework=$DST_DIR/frameworks --prefix=$DST_DIR && make clean && make && make install" )
	runCommand( "cd $DST_DIR/bin && ln -fsh python2.6 python" )
	
if depEnv["BUILD_BOOST"] :
	runCommand( "cd $BOOST_SRC_DIR; ./configure --prefix=$DST_DIR --with-python=$DST_DIR/bin/python2.6 && make clean && make && make install" )

if depEnv["BUILD_OPENEXR"] :
	runCommand( "cd $ILMBASE_SRC_DIR && ./configure --prefix=$DST_DIR && make && make install" )
	runCommand( "cd $OPENEXR_SRC_DIR && ./configure --prefix=$DST_DIR && make && make install" )

if depEnv["BUILD_JPEG"] :
	runCommand( "cd $JPEG_SRC_DIR && ./configure --prefix=$DST_DIR && make clean && make && make install-lib install-headers" )

if depEnv["BUILD_TIFF"] :
	runCommand( "cd $TIFF_SRC_DIR && ./configure --prefix=$DST_DIR && make && make install" )
		
if depEnv["BUILD_FREETYPE"] :
	runCommand( "cd $FREETYPE_SRC_DIR && ./configure --prefix=$DST_DIR && make && make install" )

if depEnv["BUILD_GLEW"] :
	runCommand( "cd $GLEW_SRC_DIR && make install GLEW_DEST=$DST_DIR" )
	
if depEnv["BUILD_CORTEX"] :
	runCommand( "cd $CORTEX_SRC_DIR; /usr/local/bin/python /opt/local/bin/scons -j 2 install INSTALL_PREFIX=$DST_DIR INSTALL_PYTHON_DIR=$DST_DIR/lib/python2.6/site-packages PYTHON_CONFIG=$DST_DIR/bin/python2.6-config BOOST_INCLUDE_PATH=$DST_DIR/include/boost-1_38 LIBPATH=$DST_DIR/lib BOOST_LIB_SUFFIX=-xgcc40-mt-1_38 OPENEXR_INCLUDE_PATH=$DST_DIR/include FREETYPE_INCLUDE_PATH=$DST_DIR/include/freetype2 RMAN_ROOT=/Applications/Graphics/3Delight-6.5.52 WITH_GL=1 GLEW_INCLUDE_PATH=$DST_DIR/include/GL DOXYGEN=/opt/local/bin/doxygen" )
	
if depEnv["BUILD_GTK"] :
	runCommand( "cd $GETTEXT_SRC_DIR && ./configure --prefix=$DST_DIR && make && make install" )
	runCommand( "cd $PKGCONFIG_SRC_DIR && ./configure --prefix=$DST_DIR && make && make install" )
	runCommand( "echo '#!/bin/sh\n' > $DST_DIR/bin/gtkdoc-rebase && chmod +x $DST_DIR/bin/gtkdoc-rebase" ) # hack to provide a stub script for glib to think it is building documentation (even though it was told not to)
	runCommand( "cd $GLIB_SRC_DIR && ./configure --prefix=$DST_DIR CPPFLAGS=-I$DST_DIR/include LDFLAGS=-L$DST_DIR/lib && make && make install" )
	runCommand( "cd $ATK_SRC_DIR && ./configure --prefix=$DST_DIR && make && make install" )
	runCommand( "cd $PIXMAN_SRC_DIR && ./configure --prefix=$DST_DIR && make && make install" )
	runCommand( "cd $PNG_SRC_DIR && ./configure --prefix=$DST_DIR && make && make install" )
	runCommand( "cd $EXPAT_SRC_DIR && ./configure --prefix=$DST_DIR && make && make install" )
	runCommand( "cd $FONTCONFIG_SRC_DIR && ./configure --prefix=$DST_DIR CPPFLAGS=-I$DST_DIR/include LDFLAGS=-L$DST_DIR/lib && make && make install" )
	runCommand( "cd $CAIRO_SRC_DIR && ./configure --prefix=$DST_DIR PKG_CONFIG=$DST_DIR/bin/pkg-config && make && make install" )
	runCommand( "cd $PANGO_SRC_DIR && ./configure --prefix=$DST_DIR PKG_CONFIG=$DST_DIR/bin/pkg-config && make clean && make && make install" )	
	runCommand( "cd $GTK_SRC_DIR && ./configure --prefix=$DST_DIR CPPFLAGS=-I$DST_DIR/include LDFLAGS=-L$DST_DIR/lib PKG_CONFIG=$DST_DIR/bin/pkg-config --without-libjasper && make && make install" )
	runCommand( "cd $PYGOBJECT_SRC_DIR && ./configure --prefix=$DST_DIR CPPFLAGS=-I$DST_DIR/include LDFLAGS=-L$DST_DIR/lib PKG_CONFIG=$DST_DIR/bin/pkg-config && make && make install" )
	runCommand( "cd $PYCAIRO_SRC_DIR && ./configure --prefix=$DST_DIR CPPFLAGS=-I$DST_DIR/include LDFLAGS=-L$DST_DIR/lib PKG_CONFIG=$DST_DIR/bin/pkg-config && make clean && make && make install" )
	runCommand( "cd $PYGTK_SRC_DIR && ./configure --prefix=$DST_DIR CPPFLAGS=-I$DST_DIR/include LDFLAGS=-L$DST_DIR/lib PKG_CONFIG=$DST_DIR/bin/pkg-config && make clean && make && make install" )
	runCommand( "cd $GTKGLEXT_SRC_DIR && ./configure --prefix=$DST_DIR CPPFLAGS=-I$DST_DIR/include LDFLAGS=-L$DST_DIR/lib PKG_CONFIG=$DST_DIR/bin/pkg-config --with-gl-includedir=/usr/X11R6/include && make clean && make && make install" )
	runCommand( "cd $PYGTKGLEXT_SRC_DIR && ./configure --prefix=$DST_DIR CPPFLAGS=-I$DST_DIR/include LDFLAGS=-L$DST_DIR/lib PKG_CONFIG=$DST_DIR/bin/pkg-config && make clean && make && make install" )
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
		"$DST_DIR/include",
		"$DST_DIR/include/boost-1_38",
		"$DST_DIR/include/OpenEXR",
	],
	
	CXXFLAGS = [
		"-Wall",
		"-Werror",
		"-O2",
	],
	
	LIBPATH = [
		"$DST_DIR/lib"
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

gafferLibraryInstall = env.Install( "$INSTALL_DIR/lib", gafferLibrary )
env.Alias( "install", gafferLibraryInstall )

uiEnv = env.Copy()
uiEnv.Append(

	LIBS = [ gafferLibrary ],

)
gafferUILibrary = uiEnv.SharedLibrary( "lib/GafferUI", glob.glob( "src/GafferUI/*.cpp" ) )
uiEnv.Default( gafferUILibrary )

gafferUILibraryInstall = env.Install( "$INSTALL_DIR/lib", gafferUILibrary )
env.Alias( "install", gafferUILibraryInstall )

###############################################################################################
# Gaffer python modules
###############################################################################################

pythonEnv = env.Copy()

pythonEnv.Append(

	CPPFLAGS = [
		"-DBOOST_PYTHON_MAX_ARITY=20",
	] + os.popen( pythonEnv.subst( "$DST_DIR/bin/python2.6-config --includes" ) ).read().split(),
	
	LIBPATH = [ "./lib" ],
	
	LIBS = [
		"boost_python" + boostLibSuffix,
		"Gaffer",
	],
	
	SHLINKFLAGS = os.popen( pythonEnv.subst( "$DST_DIR/bin/python2.6-config --ldflags" ) ).read().split(),
)

if pythonEnv["PLATFORM"]=="darwin" :
	pythonEnv.Append( SHLINKFLAGS = "-single_module" )

gafferBindingsLibrary = pythonEnv.SharedLibrary( "lib/GafferBindings", glob.glob( "src/GafferBindings/*.cpp" ) )
pythonEnv.Default( gafferBindingsLibrary )

gafferBindingsLibraryInstall = env.Install( "$INSTALL_DIR/lib", gafferBindingsLibrary )
env.Alias( "install", gafferBindingsLibraryInstall )

pythonUIEnv = pythonEnv.Copy()
pythonUIEnv.Append(
	LIBS = [ "GafferUI", "GafferBindings" ],
)
gafferUIBindingsLibrary = pythonUIEnv.SharedLibrary( "lib/GafferUIBindings", glob.glob( "src/GafferUIBindings/*.cpp" ) )
pythonUIEnv.Default( gafferUIBindingsLibrary )

gafferUIBindingsLibraryInstall = env.Install( "$INSTALL_DIR/lib", gafferUIBindingsLibrary )
env.Alias( "install", gafferUIBindingsLibraryInstall )

pythonModuleEnv = pythonEnv.Copy()
pythonModuleEnv.Append(

	LIBS = [
		"GafferBindings",
	]

)

pythonModuleEnv["SHLIBPREFIX"] = ""
pythonModuleEnv["SHLIBSUFFIX"] = ".so"

gafferModule = pythonModuleEnv.SharedLibrary( "python/Gaffer/_Gaffer", glob.glob( "src/GafferModule/*.cpp" ) )
pythonModuleEnv.Default( gafferModule )

gafferModuleInstall = env.Install( "$INSTALL_DIR/lib/python2.6/site-packages/Gaffer", gafferModule )
gafferModuleInstall += env.Install( "$INSTALL_DIR/lib/python2.6/site-packages/Gaffer", glob.glob( "python/Gaffer/*.py" ) )
env.Alias( "install", gafferModuleInstall )

pythonUIModuleEnv = pythonModuleEnv.Copy()
pythonUIModuleEnv.Append(
	LIBS = [ "GafferUI", "GafferUIBindings" ],
)
gafferUIModule = pythonUIModuleEnv.SharedLibrary( "python/GafferUI/_GafferUI", glob.glob( "src/GafferUIModule/*.cpp" ) )
pythonUIModuleEnv.Default( gafferUIModule )

gafferUIModuleInstall = env.Install( "$INSTALL_DIR/lib/python2.6/site-packages/GafferUI", gafferUIModule )
gafferUIModuleInstall += env.Install( "$INSTALL_DIR/lib/python2.6/site-packages/GafferUI", glob.glob( "python/GafferUI/*.py" ) )
env.Alias( "install", gafferUIModuleInstall )

for module in ( "GafferTest", "GafferUITest" ) :

	moduleInstall = env.Install( "$INSTALL_DIR/lib/python2.6/site-packages/" + module, glob.glob( "python/%s/*.py" % module ) )
	env.Alias( "install", moduleInstall ) 

###############################################################################################
# Scripts and apps and stuff
###############################################################################################

scriptsInstall = env.Install( "$INSTALL_DIR/bin", [ "bin/gaffer", "bin/gaffer.py" ] )
env.Alias( "install", scriptsInstall )

for app in ( "light", "view", "test", "cli" ) :
	appInstall = env.Install( "$INSTALL_DIR/apps/%s/1" % app, "apps/%s/1/%s.py" % ( app, app ) )
	env.Alias( "install", appInstall )

startupScriptsInstall = env.Install( "$INSTALL_DIR/startup/ui", glob.glob( "startup/ui/*.py" ) )
env.Alias( "install", startupScriptsInstall )

for d in ( "ui", "" ) :
	shaderInstall = env.Install( "$INSTALL_DIR/shaders/%s/" % d, glob.glob( "shaders/%s/*" % d ) )
	env.Alias( "install", shaderInstall )
	
#########################################################################################################
# Documentation
#########################################################################################################

docEnv = env.Clone()
docEnv["ENV"]["PATH"] = os.environ["PATH"]
docs = docEnv.Command( "doc/html/index.html", "doc/config/Doxyfile", "sed 's/GAFFER_MAJOR_VERSION/$GAFFER_MAJOR_VERSION/g;s/GAFFER_MINOR_VERSION/$GAFFER_MINOR_VERSION/g;s/GAFFER_PATCH_VERSION/$GAFFER_PATCH_VERSION/g' doc/config/Doxyfile | doxygen -" )
docEnv.Depends( docs, glob.glob( "include/*/*.h" ) + glob.glob( "src/*/*.cpp" ) + glob.glob( "python/*/*py" ) + glob.glob( "doc/src/*.dox" ) )
