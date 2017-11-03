#version 330 core
in vec2 uv;
out vec3 color;
uniform sampler2D fColor;
uniform sampler2D fPosition;
uniform sampler2D fNormal;
uniform sampler2D fDepth;
uniform int testBuffer;
void main()
{
	if(testBuffer == 1) // 1: position
	{
		color = texture(fPosition, uv).rgb;
	}
	else if(testBuffer == 2) // 2: normals
	{
		color = texture(fNormal, uv).rgb;
	}
	else if(testBuffer == 3) // 3: depth
	{
		float d = texture(fDepth, uv).r;
		color.r = d;
		color.g = d;
		color.b = d;
	}
	else if(testBuffer == 4) // 4: smoothness
	{
		float s = texture(fColor, uv).a;
		color.r = s;
		color.g = s;
		color.b = s;
	}
	else // 0/default: color buffer
	{
		color = texture(fColor, uv).rgb;
	}
   
}