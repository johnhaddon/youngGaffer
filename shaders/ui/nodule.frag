#include "IECoreGL/FilterAlgo.h"

void main()
{
	float r = length( gl_TexCoord[0].xy - vec2( 0.5 ) );
	float g = mix( 0.5, 0.05, ieFilteredStep( 0.35, r ) );
	float a = 1.0 - ieFilteredStep( 0.45, r );
	gl_FragColor = vec4( g, g, g, a );
}
