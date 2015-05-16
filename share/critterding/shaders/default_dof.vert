	varying float Blur;

	varying vec4 ambient;
	varying vec4 position;
	varying vec3 varyingNormalDirection;  // surface normal vector in world space
	varying vec4 ShadowCoord;

	
	void main()
	{
		ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
		ambient += gl_LightModel.ambient * gl_FrontMaterial.ambient;

		position = gl_ModelViewMatrix * gl_Vertex;
		varyingNormalDirection = normalize(gl_NormalMatrix * gl_Normal);

		vec4 position2 = gl_ModelViewProjectionMatrix * gl_Vertex;

		gl_Position = position2;

		gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;

		float focalDistance=2.0;
		float focalRange=2000.0;

     	ShadowCoord = gl_TextureMatrix[7] * gl_ModelViewMatrix * gl_Vertex;
//      	ShadowCoord = gl_TextureMatrix[7] * gl_LightSource[0].position;

		Blur = clamp(abs(-position2.z - focalDistance) / focalRange, 0.0, 1.0);
	} 














































// 	varying vec4 diffuse,ambient;
// 	varying vec3 normal,lightDir,halfVector;
// 
// 	varying float Blur;
// 	uniform float focalDistance, focalRange;
// 	
// 	void main()
// 	{	
// 		/* first transform the normal into eye space and 
// 		normalize the result */
// 		normal = normalize(gl_NormalMatrix * gl_Normal);
// 		
// 		/* now normalize the light's direction. Note that 
// 		according to the OpenGL specification, the light 
// 		is stored in eye space. Also since we're talking about 
// 		a directional light, the position field is actually direction */
// 		lightDir = normalize(vec3(gl_LightSource[0].position));
// 	
// 		/* Normalize the halfVector to pass it to the fragment shader */
// 		halfVector = normalize(gl_LightSource[0].halfVector.xyz);
// 					
// 		/* Compute the diffuse, ambient and globalAmbient terms */
// 		diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
// 		ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
// 		ambient += gl_LightModel.ambient * gl_FrontMaterial.ambient;
// 	
// 		gl_Position = ftransform();
// 		
// 		//Pass texcoord
// 		gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
// 
// 		vec4 PosWV = gl_ModelViewMatrix * gl_Vertex;
// 
// 		Blur = clamp(abs(-PosWV.z - focalDistance) / focalRange, 0.0, 1.0);
// 	} 