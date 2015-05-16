
	
	uniform sampler2D tex;

	varying float Blur;
	
	void main()
	{

		vec4 c=texture2D(tex,gl_TexCoord[0].st);
		gl_FragColor = vec4(c.rgb,Blur);
	}