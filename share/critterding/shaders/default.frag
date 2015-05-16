	varying vec4 ambient;
	varying vec4 position;
	varying vec3 varyingNormalDirection;  // surface normal vector in world space
	uniform mat4 v_inv;

	// ALTERED
	uniform sampler2D tex;
	uniform sampler2D tex1;
// 	uniform int num_lights;
// 	uniform int light_emmit;
	uniform int normal_map_do;
// 	uniform sampler2D shadow_map;
// 	varying vec4 ShadowCoord;


	mat3 cotangent_frame( vec3 N, vec3 p, vec2 uv )
	{
		// get edge vectors of the pixel triangle
		vec3 dp1 = dFdx( p );
		vec3 dp2 = dFdy( p );
		vec2 duv1 = dFdx( uv );
		vec2 duv2 = dFdy( uv );
	
		// solve the linear system
		vec3 dp2perp = cross( dp2, N );
		vec3 dp1perp = cross( N, dp1 );
		vec3 T = dp2perp * duv1.x + dp1perp * duv2.x;
		vec3 B = dp2perp * duv1.y + dp1perp * duv2.y;
	
		// construct a scale-invariant frame 
		float invmax = inversesqrt( max( dot(T,T), dot(B,B) ) );
		return mat3( T * invmax, B * invmax, N );
	}

	vec3 perturb_normal( vec3 N, vec3 V, vec2 texcoord )
	{
			// assume N, the interpolated vertex normal and 
			// V, the view vector (vertex to eye)
			vec3 map = texture2D( tex1, texcoord ).xyz;

// 		#ifdef WITH_NORMALMAP_UNSIGNED
			map = map * 255./127. - 128./127.;
// 			map = map * 2.0 - 1.0;
// 		#endif

// 		#ifdef WITH_NORMALMAP_2CHANNEL
// 			map.z = sqrt( 1. - dot( map.xy, map.xy ) );
// 		#endif

// 		#ifdef WITH_NORMALMAP_GREEN_UP
// 			map.y = -map.y;
// 		#endif

			mat3 TBN = cotangent_frame( N, -V, texcoord );
			return normalize( TBN * map );
	}

	void main()
	{
		int num_lights = 1;
// 		vec4 color = vec4(0.0, 0.0, 0.0, 0.0);
		vec4 color = ambient;

		vec3 viewDirection = normalize(vec3(v_inv * vec4(0.0, 0.0, 0.0, 1.0) - position));

		vec3 normalDirection = normalize( varyingNormalDirection );
		if ( normal_map_do == 1 )
			normalDirection = perturb_normal( normalDirection, viewDirection, gl_TexCoord[0].st );


// 		vec4 shadowCoordinateWdivide = ShadowCoord / ShadowCoord.w ;
// 		// Used to lower moiré pattern and self-shadowing
// 		shadowCoordinateWdivide.z += 0.0005;

// 		if ( light_emmit == -1 )
		{
			vec3 lightDirection;
			float attenuation;

			int light=0;

// 			for (int light=0; light < num_lights; ++light)
// 			{

// 				float distanceFromLight = texture2D(shadow_map,shadowCoordinateWdivide.st).z;
// 				float shadow = 1.0;
// 				if (ShadowCoord.w > 0.0)
// 					shadow = distanceFromLight < shadowCoordinateWdivide.z ? 0.5 : 1.0 ;



				if (0.0 == gl_LightSource[0].position.w) // directional light?
				{
					attenuation = 1.0; // no attenuation
					lightDirection = normalize(vec3(gl_LightSource[0].position));
				} 
				else // point light or spotlight (or other kind of light) 
				{
					vec3 positionToLightSource = vec3(gl_LightSource[0].position - position);
					float dist = length(positionToLightSource);
					lightDirection = normalize(positionToLightSource);
					attenuation = 1.0 / (gl_LightSource[0].constantAttenuation
										+ gl_LightSource[0].linearAttenuation * dist
										+ gl_LightSource[0].quadraticAttenuation * dist * dist);
				
					if (gl_LightSource[0].spotCutoff <= 90.0) // spotlight?
					{
						float clampedCosine = max(0.0, dot(-lightDirection, gl_LightSource[0].spotDirection));
						if (clampedCosine < cos(radians(gl_LightSource[0].spotCutoff))) // outside of spotlight cone?
						{
							attenuation = 0.0;
						}
						else
						{
							attenuation = attenuation * pow(clampedCosine, gl_LightSource[0].spotExponent);   
						}
					}
				}
				
				vec3 ambientLighting = vec3(gl_LightModel.ambient) * vec3(gl_FrontMaterial.ambient) * vec3(gl_LightSource[0].ambient);

				vec3 diffuseReflection = attenuation 
					* vec3(gl_LightSource[0].diffuse) * vec3(gl_FrontMaterial.diffuse)
					* max(0.0, dot(normalDirection, lightDirection));
				
				vec3 specularReflection;
				if (dot(normalDirection, lightDirection) < 0.0) // light source on the wrong side?
				{
					specularReflection = vec3(0.0, 0.0, 0.0); // no specular reflection
				}
				else // light source on the right side
				{
					specularReflection = attenuation * vec3(gl_LightSource[0].specular) * vec3(gl_FrontMaterial.specular) 
						* pow(max(0.0, dot(reflect(-lightDirection, normalDirection), viewDirection)), gl_FrontMaterial.shininess);
				}
	
				color += /*shadow * */vec4(ambientLighting + diffuseReflection + specularReflection, gl_FrontMaterial.diffuse.w);
// 			}

	// 		gl_FragColor = color * texture2D(tex,gl_TexCoord[0].st);
			gl_FragColor = color * texture2D( tex, gl_TexCoord[0].st/*, -2.0*/ );
		}
	}




























// 	varying vec4 diffuse,ambient;
// 	varying vec3 normal,lightDir,halfVector;
// 	
// 	uniform sampler2D tex;
// 	
// 	void main()
// 	{
// 		vec3 n,halfV;
// 		float NdotL,NdotHV;
// 		
// 		/* The ambient term will always be present */
// 		vec4 color = ambient;
// 		
// 		/* a fragment shader can't write a varying variable, hence we need
// 		a new variable to store the normalized interpolated normal */
// 		n = normalize(normal);
// 		
// 		/* compute the dot product between normal and ldir */
// 		NdotL = max(dot(n,lightDir),0.0);
// 	
// 		if (NdotL > 0.0) {
// 			color += diffuse * NdotL;
// 			halfV = normalize(halfVector);
// 			NdotHV = max(dot(n,halfV),0.0);
// 			color += gl_FrontMaterial.specular * 
// 					gl_LightSource[0].specular * 
// 					pow(NdotHV, gl_FrontMaterial.shininess);
// 		}
// 	
// 		vec4 c=color * texture2D(tex,gl_TexCoord[0].st);
// 		gl_FragColor = c;
// 	}