from IECore import Enum

Caps = Enum.create( "Unchanged", "First", "All", "AllExceptFirst" )

def split( camelCase ) :

	if not len( camelCase ) :
		return []
	
	# split into words based on adjacent cases being the same	
	result = []
	current = ""
	prevUpper = camelCase[0].isupper()
	for c in camelCase :
		upper = c.isupper()
		if upper==prevUpper :
			current += c
		else :
			result.append( current )
			current = c
		prevUpper = upper
		
	result.append( current )
	
	# move last capital of previous word onto any lowercase words
	i = 1
	while i<len( result ) :
	
		if result[i].islower() and result[i-1][-1].isupper() :
			
			result[i] = result[i-1][-1] + result[i]
			if len( result[i-1] )==1 :
				del result[i-1]
				i-=1
			else :
				result[i-1] = result[i-1][:-1]
				
		i+=1
	
	return result
	
def join( words, caps=Caps.All, separator="" ) :

	cWords = []
	for i in range( 0, len( words ) ) :
		word = words[i]
		if caps!=Caps.Unchanged :
			if (caps==Caps.First and i==0) or caps==Caps.All or (caps==Caps.AllExceptFirst and i!=0) :
				if not word.isupper() :
					word = word.lower()
				word = word[0].upper() + word[1:]
			elif caps==Caps.AllExceptFirst and i==0 or (caps==Caps.First and i!=0):
				word = word.lower()
						
		cWords.append( word )
		
	return separator.join( cWords )

## Convert a CamelCase word to a string with spaces between words
def toSpaced( camelCase, caps=Caps.All ) :

	s = split( camelCase )
	return join( s, caps, " " )
	
	
## Convert a spaced word to a camel case string	
def fromSpaced( spaced, caps=Caps.All ) :

	s = spaced.split()
	return join( s, caps )
	
