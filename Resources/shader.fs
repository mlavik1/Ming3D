#version 430

layout (location=0) in vec3 Position;
layout (location=1) in vec3 Normal;
layout (location=2) in vec2 TexCoord;

uniform sampler2D inTexture;

out vec4 FragColour;

void main()
{
	//FragColour = vec4(0.0, 1.0, 0.0, 1.0);
	//FragColour = vec4(Normal, 1.0);
	FragColour = texture(inTexture, TexCoord);
}