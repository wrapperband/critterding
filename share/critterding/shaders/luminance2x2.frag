uniform int Width;
uniform int Height;
uniform sampler2D Tex0;

const vec2 offset2x2 = vec2(-0.5, 0.5);
const vec3 LUMINANCE = vec3(0.299, 0.587, 0.114);

void main(void)
{
	int x, y;
	float average = 0.0;
	float maximum = -1e20;
	vec4 color = vec4(0.0);

	for(x=0; x<2; x++) {
		for(y=0; y<2; y++) {
			vec2 offset = vec2(offset2x2[x] * (1.0/float(Width)), offset2x2[y] * (1.0/float(Height)));
			color = texture2D(Tex0, gl_TexCoord[0].st + offset);

			float GreyValue = dot(color.rgb, LUMINANCE);
			maximum = max(maximum, GreyValue);
			average += log(1e-5 + GreyValue) / 4.0;
		}
	}

	average = exp(average);
	gl_FragColor = vec4(average, maximum, 0.0, 1.0);
}
