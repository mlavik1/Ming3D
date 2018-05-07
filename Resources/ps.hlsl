Texture2D shaderTexture;
SamplerState SampleType;

float4 main(float4 position : SV_POSITION, float2 texCoord : TEXCOORD) : SV_TARGET
{
	float4 texColour = shaderTexture.Sample(SampleType, texCoord);
	
    return texColour;
}
