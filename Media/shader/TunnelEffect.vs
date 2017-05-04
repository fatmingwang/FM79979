attribute vec3 VSPosition;
attribute vec4 VSColor;
attribute vec2 VSTexcoord;
uniform mat4 matVP;
uniform mat4 matW;
varying vec2 PSTexcoord;
varying vec4 PSColor;
uniform float time;

void main()
{
	gl_Position = matVP*matW*vec4(VSPosition,1);
//                  gl_Position.z = sin(time)*30;
                  if( gl_Position.z <0 )
                     gl_Position.z *= -1;
	PSTexcoord = VSTexcoord;
	PSColor = VSColor;
}