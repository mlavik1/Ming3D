Texture2D shaderTexture;
SamplerState SampleType;

struct VIn
{
    float4 position : SV_POSITION;
	float3 texCoord2 : NORMAL;
	float2 texCoord : TEXCOORD;
};

float4 main(VIn input) : SV_TARGET
{
	float4 texColour = shaderTexture.Sample(SampleType, input.texCoord);
	
    return texColour;
}
