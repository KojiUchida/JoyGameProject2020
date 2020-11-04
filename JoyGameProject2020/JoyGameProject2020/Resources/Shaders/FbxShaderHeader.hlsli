SamplerState smp : register(s0);

cbuffer sceneBuffer : register(b0) {
	matrix world;
	matrix rot;
	matrix view;
	matrix proj;
	matrix mvp;
	float3 lightDir;
};

struct VSOutput {
	float4 svpos : SV_POSITION;
	float4 normal : NORMAL;
	float2 uv : TEXCOORD;
};
