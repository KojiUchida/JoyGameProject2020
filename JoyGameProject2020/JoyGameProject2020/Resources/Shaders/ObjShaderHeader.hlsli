Texture2D<float4> ambientTex : register(t0);
Texture2D<float4> diffuseTex : register(t1);
Texture2D<float4> specularTex : register(t2);
Texture2D<float4> bumpTex : register(t3);

SamplerState smp : register(s0);

cbuffer sceneBuffer : register(b0) {
	matrix world;
	matrix rot;
	matrix view;
	matrix proj;
	matrix mvp;
	float3 lightDir;
	float3 texturepos;
	float2 textureWH;
};

cbuffer materialBuffer : register(b1) {
	float3 ambientColor;
	float alpha;
	float3 diffuseColor;
	float shininess;
	float3 specularColor;
};

struct VSOutput {
	float4 svpos : SV_POSITION;
	float4 normal : NORMAL;
	float2 uv : TEXCOORD;
};
