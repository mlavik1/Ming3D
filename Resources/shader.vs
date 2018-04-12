#version 430

layout (location=0) in vec3 VertexPosition;
layout (location=1) in vec3 VertexNormal;

uniform mat4 MVP;
uniform vec4 test;

out vec3 Position;
out vec3 Normal;

void main()
{
	Position = VertexPosition;
	Normal = VertexNormal;
	gl_Position = MVP * vec4(VertexPosition, 1.0);
}