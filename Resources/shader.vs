#version 430

layout (location=0) in vec3 VertexPosition;
layout (location=1) in vec3 VertexNormal;
layout (location=2) in vec2 VertexTexCoord;

uniform mat4 MVP;
uniform vec4 test;

layout (location=0) out vec3 Position;
layout (location=1) out vec3 Normal;
layout (location=2) out vec2 TexCoord;

void main()
{
	Position = VertexPosition;
	Normal = VertexNormal;
	TexCoord = VertexTexCoord;
	gl_Position = MVP * vec4(VertexPosition, 1.0);
}