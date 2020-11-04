Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

cbuffer sceneBuffer : register(b0) {
	matrix mat;
	matrix mvp;
};

struct VSOutput {
	float4 pos : POSITION;
};

struct GSOutput {
	float4 svpos : SV_POSITION;
	float2 uv : TEXCOORD;
};
