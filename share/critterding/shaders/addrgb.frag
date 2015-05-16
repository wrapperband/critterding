uniform sampler2D Tex0;
uniform sampler2D Tex1;

void main(void)
{
	vec4 color = texture2D(Tex0, gl_TexCoord[0].st);
	color.rgb += texture2D(Tex1, gl_TexCoord[0].st).rgb;
	
	gl_FragColor = color;
}
