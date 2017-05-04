attribute vec3 VSPosition;
attribute vec4 VSColor;
attribute vec2 VSTexcoord;
uniform mat4 matVP;
uniform mat4 matW;
varying vec2 PSTexcoord;
varying vec4 PSColor;


void main()
{
	gl_Position = matVP*matW*vec4(VSPosition,1);
	PSTexcoord = VSTexcoord;
	PSColor = VSColor;
}