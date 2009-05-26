void main()
{
	float r = length( gl_TexCoord[0].xy - vec2( 0.5 ) );

	/// when this is used as the alpha we get circles but
	/// then selection mode stops working
	float a = 1.0 - smoothstep( 0.45, 0.5, r );

	gl_FragColor = vec4( 0.2, 0.2, 0.2, 1 );
}
