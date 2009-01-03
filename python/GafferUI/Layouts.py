import GafferUI

__namedLayouts = {}

## Serialises the passed Editor and stores it using the given name. This
# layout can then be recreated using the create() method below.
def add( name, editor ) :

	__namedLayouts[name] = repr( editor )

## Removes a layout previously stored with add().
def remove( name ) :

	del __namedLayouts[name]

## Returns a list of the names of currently defined layouts
def names() :

	return __namedLayouts.keys()

## Recreates a previously stored layout, returning it in the form of a CompoundEditor.d
def create( name ) :

	r = __namedLayouts[name]
	return eval( r )

## Saves all layouts whose name matches the optional regular expression into the file object
# specified. If the file is later evaluated it will reregister the layouts with this module.
def save( fileObject, nameRegex = None ) :

	for name in names() :
		if nameRegex.match( name ) or nameRegex is None :
			fileObject.write( "GafferUI.Layouts.add( \"%s\", %s )\n\n" % ( name, __namedLayouts[name] ) )
	
