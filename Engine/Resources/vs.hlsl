cbuffer SHADER_CONSTANT_BUFFER : register(b0)
{
    matrix MVP;
	float4 test;
};

struct VIn
{
    float4 VertexPosition : POSITION;
	float3 VertexNormal : NORMAL;
	float2 VertexTexCoord : TEXCOORD;
};

struct VOut
{
    float4 position : SV_POSITION;
	float3 texCoord2 : NORMAL;
	float2 texCoord : TEXCOORD;
};

VOut main(VIn input)
{
    VOut output;

    output.position = mul(input.VertexPosition, MVP);
	//output.color = float4(VertexTexCoord.x, VertexTexCoord.y, 0.0f, 1.0f);

	output.texCoord = input.VertexTexCoord;
	
    return output;
}
