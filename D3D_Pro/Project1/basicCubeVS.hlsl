cbuffer cbPerFrame: register(b0)
{
	float4x4 matVP; 
	float4x4 matGeo;
};

struct VSInput
{
	float3 Position : POSITION; 
	float4 Color : COLOR; 
};

struct VSOutput
{
	float4 Position : SV_POSITION; 
	float4 Color : COLOR; 
};

VSOutput main(VSInput vIn)
{
	VSOutput vOut = (VSOutput)0;
	vOut.Position = mul(mul(float4(vIn.Position , 1.0f) , matGeo),  matVP); 
	vOut.Color = vIn.Color;

	return vOut; 
}