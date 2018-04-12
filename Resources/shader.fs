#version 430

in vec3 Position;
in vec3 Normal;

out vec4 FragColour;

void main()
{
	FragColour = vec4(0.0, 1.0, 0.0, 1.0);
	FragColour = vec4(Normal, 1.0);
	//FragColour = vec4(0.0, Position.x, 0.0, 1.0);
}