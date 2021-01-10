Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

cbuffer sceneBuffer : register(b0) {
	matrix world;
	matrix rot;
	matrix view;
	matrix proj;
	matrix mvp;
	float3 lightDir;
	float num;
};

struct VSOutput {
	float4 svpos : SV_POSITION;
	float4 normal : NORMAL;
	float2 uv : TEXCOORD;
};


VSOutput VSmain(float4 pos : POSITION, float4 normal : NORMAL, float2 uv : TEXCOORD) {
	
	VSOutput output;
	output.svpos = mul(mvp, pos);
	output.normal = mul(rot, normal);

	float left=(num * 64) / 640;
	float top = 0;
	float width = 64 / 640;
	float height = 1;

	output.uv = uv * float2(width, height) + float2(left, top);

	return output;
}

float4 PSmain(VSOutput input) : SV_TARGET{
	
	float4 texcolor = tex.Sample(smp,input.uv);

	return texcolor;
}