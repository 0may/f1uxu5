precision mediump float;

attribute vec4 position;		// set automatically by OF
attribute vec2 texcoord;		// set automatically by OF 
uniform mat4 modelViewProjectionMatrix; // set automatically by OF 

//our variables
varying vec2 texcoord0;

void main()
{
	gl_Position = modelViewProjectionMatrix * position;

	//we copy the internal texcoords so we can manipulate them
	//this is essentially the internal structure of the image
	texcoord0 = texcoord;
}

