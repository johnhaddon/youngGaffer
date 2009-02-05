import os
import glob

boostLibSuffix = "-mt-1_35"

env = Environment(

	GAFFER_MAJOR_VERSION = "0",
	GAFFER_MINOR_VERSION = "1",
	GAFFER_PATCH_VERSION = "0",

	CPPPATH = [
		"include",
		"/opt/local/include",
		"/opt/local/include/OpenEXR",
		"/opt/local/include/boost-1_35",
	],
	
	CXXFLAGS = [
		"-Wall",
		"-Werror",
		"-O2",
	],
	
	LIBPATH = [
		"/opt/local/lib"
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

uiEnv = env.Copy()
uiEnv.Append(

	LIBS = [ gafferLibrary ],

)
gafferUILibrary = uiEnv.SharedLibrary( "lib/GafferUI", glob.glob( "src/GafferUI/*.cpp" ) )
uiEnv.Default( gafferUILibrary )

pythonEnv = env.Copy()

pythonEnv.Append(

	CPPFLAGS = [
		"-DBOOST_PYTHON_MAX_ARITY=20",
	] + os.popen( "python-config --includes" ).read().split(),	
	
	LIBPATH = [ "./lib" ],
	
	LIBS = [
		"boost_python" + boostLibSuffix,
		"Gaffer",
	],
	
	SHLINKFLAGS = os.popen( "python-config --ldflags" ).read().split(),
)

if pythonEnv["PLATFORM"]=="darwin" :
	pythonEnv.Append( SHLINKFLAGS = "-single_module" )

gafferBindingsLibrary = pythonEnv.SharedLibrary( "lib/GafferBindings", glob.glob( "src/GafferBindings/*.cpp" ) )
pythonEnv.Default( gafferBindingsLibrary )

pythonUIEnv = pythonEnv.Copy()
pythonUIEnv.Append(
	LIBS = [ "GafferUI", "GafferBindings" ],
)
gafferUIBindingsLibrary = pythonUIEnv.SharedLibrary( "lib/GafferUIBindings", glob.glob( "src/GafferUIBindings/*.cpp" ) )
pythonUIEnv.Default( gafferUIBindingsLibrary )

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

pythonUIModuleEnv = pythonModuleEnv.Copy()
pythonUIModuleEnv.Append(
	LIBS = [ "GafferUI", "GafferUIBindings" ],
)
gafferUIModule = pythonUIModuleEnv.SharedLibrary( "python/GafferUI/_GafferUI", glob.glob( "src/GafferUIModule/*.cpp" ) )
pythonUIModuleEnv.Default( gafferUIModule )


#########################################################################################################
# Documentation
#########################################################################################################

docEnv = env.Clone()
docEnv["ENV"]["PATH"] = os.environ["PATH"]
docs = docEnv.Command( "doc/html/index.html", "doc/config/Doxyfile", "sed 's/GAFFER_MAJOR_VERSION/$GAFFER_MAJOR_VERSION/g;s/GAFFER_MINOR_VERSION/$GAFFER_MINOR_VERSION/g;s/GAFFER_PATCH_VERSION/$GAFFER_PATCH_VERSION/g' doc/config/Doxyfile | doxygen -" )
docEnv.Depends( docs, glob.glob( "include/*/*.h" ) + glob.glob( "src/*/*.cpp" ) + glob.glob( "python/*/*py" ) + glob.glob( "doc/src/*.dox" ) )
