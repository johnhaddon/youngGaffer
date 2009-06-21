#include "IECoreGL/FilterAlgo.h"

uniform vec2 cornerSizes;

void main()
{
	
	vec2 v = max( cornerSizes - gl_TexCoord[0].xy, vec2( 0.0 ) ) + max( gl_TexCoord[0].xy - vec2( 1.0 ) + cornerSizes, vec2( 0.0 ) );
	v /= cornerSizes;	
	float r = length( v );
	
	vec3 c = mix( gl_Color.rgb, vec3( 0.05 ), ieFilteredStep( 0.8, r ) );
	float a = 1.0 - ieFilteredStep( 1.0, r );
	gl_FragColor = vec4( c, a );

}
