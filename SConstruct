import os
import glob

env = Environment(

	CPPPATH = [
		"include",
		"/opt/local/include",
		"/opt/local/include/OpenEXR",
		"/opt/local/include/boost-1_34_1",
	],
	
	CXXFLAGS = [
		"-Wall",
		"-Werror"
	],
	
	LIBPATH = [
		"/opt/local/lib"
	],
	
	LIBS = [
		"boost_signals-1_34_1",
		"IECore",
	],

)

gafferLibrary = env.SharedLibrary( "lib/Gaffer", glob.glob( "src/Gaffer/*.cpp" ) )

uiEnv = env.Copy()
uiEnv.Append(

	LIBS = [ gafferLibrary ],

)
gafferUILibrary = uiEnv.SharedLibrary( "lib/GafferUI", glob.glob( "src/GafferUI/*.cpp" ) )

pythonEnv = env.Copy()

pythonEnv.Append(

	CPPFLAGS = [
		"-DBOOST_PYTHON_MAX_ARITY=20",
	] + os.popen( "python-config --includes" ).read().split(),	
	
	LIBPATH = [ "./lib" ],
	
	LIBS = [
		"boost_python-1_34_1",
		"Gaffer",
	],
	
	SHLINKFLAGS = os.popen( "python-config --ldflags" ).read().split(),
)

if pythonEnv["PLATFORM"]=="darwin" :
	pythonEnv.Append( SHLINKFLAGS = "-single_module" )

gafferBindingsLibrary = pythonEnv.SharedLibrary( "lib/GafferBindings", glob.glob( "src/GafferBindings/*.cpp" ) )

pythonUIEnv = pythonEnv.Copy()
pythonUIEnv.Append(
	LIBS = [ "GafferUI", "GafferBindings" ],
)
gafferUIBindingsLibrary = pythonUIEnv.SharedLibrary( "lib/GafferUIBindings", glob.glob( "src/GafferUIBindings/*.cpp" ) )

pythonModuleEnv = pythonEnv.Copy()
pythonModuleEnv.Append(

	LIBS = [
		"GafferBindings",
	]

)

pythonModuleEnv["SHLIBPREFIX"] = ""
pythonModuleEnv["SHLIBSUFFIX"] = ".so"

pythonModuleEnv.SharedLibrary( "python/Gaffer/_Gaffer", glob.glob( "src/GafferModule/*.cpp" ) )

pythonUIModuleEnv = pythonModuleEnv.Copy()
pythonUIModuleEnv.Append(
	LIBS = [ "GafferUI", "GafferUIBindings" ],
)
pythonUIModuleEnv.SharedLibrary( "python/GafferUI/_GafferUI", glob.glob( "src/GafferUIModule/*.cpp" ) )
