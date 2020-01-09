uniform mat4 MVP;
uniform mat4 modelViewMat;

#ifdef use_mat_colour
    uniform vec4 colourDiffuse;
#endif
    uniform vec4 colourSpecular;
    uniform float shininess;


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
    vec4 Normal : NORMAL;
    vec2 TexCoord : TEXCOORD;
    vec4 WorldPosition : POSITION;
}

// Vertex shader
shader VertexShader
{
    void main(VSInput input, FSInput output)
    {
        output.Position = MVP * input.Position;
        output.Normal = modelViewMat * vec4(input.Normal.x, input.Normal.y, input.Normal.z, 0.0);
        output.TexCoord = input.TexCoord;
        output.WorldPosition = modelViewMat * input.Position; 
    }
}

// Fragment shader
shader FragmentShader
{
    void main(FSInput input)
    {
        vec4 col = vec4(0.0, 0.0, 0.0, 0.0);
    #ifdef use_mat_colour
        col = colourDiffuse;
    #else
        col = ReadTexture(inTexture, input.TexCoord);
    #endif
        
    #ifndef unlit_mode
        vec3 n = normalize(input.Normal.xyz);
        float ndotl = max(dot(n, _lightDir * -1.0), 0.0);
        vec3 diffuse =  ndotl * vec3(_lightCol.x * col.x, _lightCol.y * col.y, _lightCol.z * col.z);
        vec3 ambient = col.xyz * 0.2; // TODO
        vec3 v = normalize(_eyePos - input.WorldPosition.xyz);
        vec3 r = normalize(reflect(_lightCol.xyz * -1.0, n));
        float rdotv = max( dot( r, v ), 0.0 );
        vec3 specular = pow(rdotv, shininess) * colourSpecular.xyz * col.xyz;

        col = vec4(diffuse.x + ambient.x + specular.x, diffuse.y + ambient.y + specular.y, diffuse.z + ambient.z + specular.z, 1.0);
    #endif

        SetFragmentColour(col);
    }
}

