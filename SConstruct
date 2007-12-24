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

pythonEnv["SHLIBPREFIX"] = ""
pythonEnv["SHLIBSUFFIX"] = ".so"

if pythonEnv["PLATFORM"]=="darwin" :
	pythonEnv.Append( SHLINKFLAGS = "-single_module" )

pythonEnv.SharedLibrary( "python/Gaffer/_Gaffer", glob.glob( "src/GafferBindings/*.cpp" ) )
