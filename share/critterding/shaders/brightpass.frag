// // varying vec2 texCoord;
// uniform sampler2D Tex0;
// uniform sampler2D Tex1;
// // uniform vec2	samples[16];
// // uniform	float	key;
// const float  BRIGHT_PASS_THRESHOLD  = 2.5;  // Threshold for BrightPass filter
// const float  BRIGHT_PASS_OFFSET     = 1.0; // Offset for BrightPass filter
// 
// void main(void)
// {
// 	vec3 col = vec3(0.0, 0.0, 0.0);
// // 	for(int i = 0; i < 16; i++)
// 	{
// 		col += texture2D(Tex0, gl_TexCoord[0].st/* + samples[i]).xyz*/;
// 	}
// 	col *= 0.0625;
// 	
// 	float  ALum = texture2D(Tex1, vec2(0.5, 0.5) ).r;
// 
// 	// Determine what the pixel's value will be after tone-mapping occurs
// 	float key = 1.0;
// 	col.xyz *= key/(ALum + 0.001);
// 	
// 	// Subtract out dark pixels
// 	col.xyz -= BRIGHT_PASS_THRESHOLD;
// 	
// 	// Clamp to 0
// 	col = max(col, 0.0);
// 	
// 	// Map the resulting value into the 0 to 1 range. Higher values for
// 	// BRIGHT_PASS_OFFSET will isolate lights from illuminated scene 
// 	// objects.
// 	col.xyz /= (BRIGHT_PASS_OFFSET + col.xyz);
// 
// 	gl_FragColor = vec4(col, 1.0);
// 
// }






// uniform sampler2D Tex0;
// uniform sampler2D Tex1;
// const float  BRIGHT_PASS_THRESHOLD  = 2.2;  // Threshold for BrightPass filter
// const float  BRIGHT_PASS_OFFSET     = 1.5; // Offset for BrightPass filter
// 
// void main(void)
// {
// 	float lum, scaleLum;
// 
// 	vec4 color = texture2D(Tex0, gl_TexCoord[0].st);
// 	color.rbg *= 0.90;
// 	vec4 lc = texture2D(Tex1, vec2(0.5, 0.5) ).r;
// 
// 	// Determine what the pixel's value will be after tone-mapping occurs
// 	float key = 2.0;
// 	color.xyz *= key/(lc + 0.001);
// 
// 	// Subtract out dark pixels
// 	color.xyz -= BRIGHT_PASS_THRESHOLD;
// 
// 	// Clamp to 0
// 	color.xyz = max(color.xyz, 0.0);
// 
// 	// Map the resulting value into the 0 to 1 range. Higher values for
// 	// BRIGHT_PASS_OFFSET will isolate lights from illuminated scene 
// 	// objects.
// 	color.xyz /= (BRIGHT_PASS_OFFSET + color.xyz);
// 	gl_FragColor = color;
// // 	gl_FragColor = vec4(color, 1.0);
// }








uniform float Exposure;
uniform float Threshold;

uniform sampler2D Tex0;
uniform sampler2D Tex1;

// const vec3 LUMINANCE = vec3(0.299, 0.587, 0.114);
const vec3 LUMINANCE = vec3(0.799, 0.987, 0.514);

void main(void)
{
	float lum, scaleLum;

	vec4 color = texture2D(Tex0, gl_TexCoord[0].st);
// 	vec4 lc = texture2D(Tex1, vec2(0.5, 0.5));
// 
// 	lum = dot(color.rgb, LUMINANCE);
// 
// 	scaleLum = (lum * Exposure) / lc.r;
// 	color.rgb *= (scaleLum * (1.0 + (scaleLum / (lc.g * lc.g)))) / (1.0 + scaleLum);
// 	color.rgb -= Threshold;
// 	color.rgb = max(color.rgb, 0.0);

// 	color.xyz /= (1.0 + color.xyz);

	gl_FragColor = color;
}
