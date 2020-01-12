ShaderUniforms
{
	mat4 MVP;
	vec4 test;
	vec4 colour;
}

// Vertex shader input
struct VSInput
{
	vec4 Position : POSITION;
	vec3 Normal : NORMAL;
	vec2 TexCoord : TEXCOORD;
}

// Fragment shader input
struct FSInput
{
	vec4 Position : SV_POSITION;
	vec3 Normal : NORMAL;
	vec2 TexCoord : TEXCOORD;
}

// Vertex shader
shader VertexShader
{
	void main(VSInput input, FSInput output)
	{
		output.Position = test + MVP * input.Position;
		output.Normal = input.Normal; // TODO
		output.TexCoord = input.TexCoord;
	}
}

// Fragment shader
shader FragmentShader
{
	void main(FSInput input)
	{
		SetFragmentColour(colour);
	}
}

