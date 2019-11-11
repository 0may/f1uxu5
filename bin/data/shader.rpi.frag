precision mediump float;

//we passed this in from our vert shader
varying vec2        texcoord0;

uniform sampler2D   tex0;
uniform sampler2D   texFbo;

uniform vec2        resolution;

uniform float       alpha;
uniform vec2        offset;


float intensity(vec4 c) {
	return 0.299*c.r + 0.587*c.g + 0.114*c.b;
}


void main()
{

//	vec2 stFbo = vec2((texcoord0.x - offset.x/resolution.x), (texcoord0.y - offset.y/resolution.y));
//	vec4 cFbo = vec4(0.0, 0.0, 0.0, 1.0);
//
//	if (stFbo.x >= 0.0 && stFbo.x < resolution.x && stFbo.y >= 0.0 && stFbo.y < resolution.y) {
//		cFbo = vec4( texture2D(texFbo, stFbo.st).rgb * alpha, 1.0);
//	}

	vec4 cCam = texture2D(tex0, texcoord0.st);
	vec4 cFbo = vec4( texture2D(texFbo, vec2((texcoord0.x-offset.x/resolution.x), (texcoord0.y-offset.y/resolution.y))).rgb * alpha, 1.0);


	if (intensity(cCam) > intensity(cFbo)) {
		gl_FragColor = cCam;
	}
	else {
		gl_FragColor = cFbo;
	}
}


