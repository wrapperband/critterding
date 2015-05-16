	varying vec4 ambient;
	varying vec4 position;
	varying vec3 varyingNormalDirection;  // surface normal vector in world space
// 	varying vec4 ShadowCoord;

	void main()
	{
		ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
		ambient += gl_LightModel.ambient * gl_FrontMaterial.ambient;

		position = gl_ModelViewMatrix * gl_Vertex;
		varyingNormalDirection = normalize(gl_NormalMatrix * gl_Normal);
// 		gl_Position = ftransform();
		gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
		gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
//      	ShadowCoord = gl_TextureMatrix[7] * gl_ModelViewMatrix * gl_Vertex;
	}
