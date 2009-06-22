#include "IECoreGL/FilterAlgo.h"

void main()
{
	float a = ieFilteredPulse( 0.2, 0.8, gl_TexCoord[0].x );
	gl_FragColor = vec4( 0.15, 0.15, 0.15, a );
}
