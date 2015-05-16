

	varying float Blur;
	uniform float focalDistance, focalRange;
	
	void main()
	{	
	
		gl_Position = ftransform();
		
		//Pass texcoord
		gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;

		vec4 PosWV = gl_ModelViewMatrix * gl_Vertex;

		Blur = clamp(abs(-PosWV.z - focalDistance) / focalRange, 0.0, 1.0);
	} 