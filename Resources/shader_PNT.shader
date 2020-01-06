uniform mat4 MVP;

#ifdef use_mat_colour
    uniform vec4 colour;
#endif


cbuffer _Globals
{
    vec3 _lightDir;
    vec4 _lightCol;
    vec3 _eyePos;
    float _time;
}

ShaderTextures
{
	Texture2D inTexture;
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
		output.Position = MVP * input.Position;
		output.Normal = input.Normal;
		output.TexCoord = input.TexCoord;
	}
}

// Fragment shader
shader FragmentShader
{
	void main(FSInput input)
	{
        vec4 col = vec4(0.0, 0.0, 0.0, 0.0);
    #ifdef use_mat_colour
        col = colour;
    #else
		col = ReadTexture(inTexture, input.TexCoord);
    #endif

    SetFragmentColour(col);
	}
}

