uniform float Offsets[9];
uniform float Weights[9];
uniform sampler2D Tex0;

void main (void)
{
	int i;
	vec4 color = vec4(0.0, 0.0, 0.0, 1.0);

	for(i=0; i<9; i++) {
		color += (texture2D(Tex0, gl_TexCoord[0].st + vec2(0.0, Offsets[i])) * Weights[i]);
	}

	gl_FragColor = color;
}
