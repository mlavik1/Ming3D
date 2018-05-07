cbuffer VS_CONSTANT_BUFFER : register(b0)
{
    matrix MVP;
	float4 test;
};

struct VOut
{
    float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD;
};

VOut main(float4 position : POSITION, float4 normal : NORMAL, float2 texCoord : TEXCOORD)
{
    VOut output;

    output.position = mul(position, MVP);
    //output.position = position;
	//output.color = color;
	//output.color = float4(normal.x, normal.y, normal.z, 1.0f);

	//output.color = float4(texCoord.x, texCoord.y, 0.0f, 1.0f);

	output.texCoord = texCoord;
	
    return output;
}
