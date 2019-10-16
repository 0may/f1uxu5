// fragment shader

#version 150

uniform float alpha;
uniform vec2 offset;
uniform vec2 size;
uniform sampler2DRect tex0;
uniform sampler2DRect texFbo;

out vec4 outputColor;

float intensity(vec4 c) {
	return 0.299*c.r + 0.587*c.g + 0.114*c.b;
}

void main()
{
	vec2 stFbo = vec2((gl_FragCoord.x - offset.x), (gl_FragCoord.y - offset.y));

	vec4 cFbo = vec4(0.0, 0.0, 0.0, 1.0);
	
	if (stFbo.x >= 0 && stFbo.x < size.x && stFbo.y >= 0 && stFbo.y < size.y) {
		cFbo = vec4( texture(texFbo, stFbo).rgb * alpha, 1.0);
	}

	vec4 cCam = texture(tex0, gl_FragCoord.xy);

	if (intensity(cCam) > intensity(cFbo))
		outputColor = cCam;
	else
		outputColor = cFbo;
}


