attribute vec3 VSPosition;
attribute vec4 VSColor;
attribute vec2 VSTexcoord;
uniform mat4 matVP;
uniform mat4 matW;
uniform vec2 PixelOffset;//   = vec2( 0.001953195, 0.00260416666 );// Pixel offsets ( 1 / 512, 1 / 384 )
varying vec2 PSTexcoord;
varying vec4 PSColor;

void main()
{
	gl_Position = matVP*matW*vec4(VSPosition,1);
	PSTexcoord = VSTexcoord+( PixelOffset * 0.5 );
	PSColor = VSColor;
}