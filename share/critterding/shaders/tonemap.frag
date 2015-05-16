uniform float Exposure;
// uniform float Threshold;
uniform sampler2D Tex0;
uniform sampler2D Tex1;

// // DISABLED
// void main(void)
// {
// // 	float Lp, LmSqr, toneScalar;
// // 	float fGaussianScalar = 0.0;
// 	vec4 color = texture2D(Tex0, gl_TexCoord[0].st);
// // 	vec4 l = texture2D(Tex1, vec2(0.5, 0.5));
// // 	Lp = ( Exposure / l.r ) * max( color.r, max( color.g, color.b ) );
// // 	LmSqr = (l.g/* + fGaussianScalar*/ * l.g) * (l.g/* + fGaussianScalar*/ * l.g);
// // 	toneScalar = ( Lp * ( 1.0 + ( Lp / ( LmSqr ) ) ) ) / ( 1.0 + Lp );
// 
// // // 	toneScalar = 0.5 * (toneScalar+1.0);
// // 	if ( toneScalar > 0..9 )
// // 	color.rgb *= toneScalar;
// 
// // 	gl_FragColor = vec4(color.r, color.g, color.b, 1.0);
//     gl_FragColor = color;
// }


// // TEST
// void main(void)
// {
// 	float Lp, LmSqr, toneScalar;
// 	float fGaussianScalar = 0.0;
// 	vec4 color = texture2D(Tex0, gl_TexCoord[0].st);
// 	vec4 l = texture2D(Tex1, vec2(0.5, 0.5));
// 	Lp = ( Exposure / l.r ) * max( color.r, max( color.g, color.b ) );
// 	LmSqr = (l.g/* + fGaussianScalar*/ * l.g) * (l.g/* + fGaussianScalar*/ * l.g);
// 	toneScalar = ( Lp * ( 1.0 + ( Lp / ( LmSqr ) ) ) ) / ( 1.0 + Lp );
// 
// // // 	toneScalar = 0.5 * (toneScalar+1.0);
// // 	if ( toneScalar > 0..9 )
// 	color.rgb *= toneScalar;
// 
// // 	gl_FragColor = vec4(color.r, color.g, color.b, 1.0);
//     gl_FragColor = color;
// }




// ORIGINAL

void main(void)
{
	float Lp, LmSqr, toneScalar;

	vec4 color = texture2D(Tex0, gl_TexCoord[0].st);
	vec4 l = texture2D(Tex1, vec2(0.5, 0.5));

	Lp = (Exposure / l.r) * max(color.r, max(color.g, color.b));

	LmSqr = (l.g * l.g) * (l.g * l.g);
	toneScalar = (Lp * (1.0 + (Lp / (LmSqr)))) / (1.0 + Lp);



// 	if ( toneScalar > 2.0 )
// 		toneScalar=2.0;

// 	if ( toneScalar > 1.0 )
// 		toneScalar=sqrt(toneScalar);


	if ( toneScalar < 1.0 )
		toneScalar=1.0;

	color.rgb = (color.rgb + ( color.rgb*toneScalar ))/2;


//     color.rgb *= toneScalar;

    gl_FragColor = color;
}
