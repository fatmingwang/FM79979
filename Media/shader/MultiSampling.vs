#version 400

in vec3 VSPosition;
in vec4 VSColor;

in vec2 VSTexcoord;

uniform mat4 matVP;									
uniform mat4 matW;	
								
out vec2 PSTexcoord;								
out vec4 PSColor;
								
void main()
{														
     gl_Position = matVP*matW*vec4(VSPosition,1);		
     PSTexcoord = VSTexcoord;						
    PSColor = VSColor;									
}