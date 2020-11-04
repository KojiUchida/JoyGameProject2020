#include "FbxShaderHeader.hlsli"

float4 PSmain(VSOutput input) : SV_TARGET {
	float intensity = saturate(dot(normalize(input.normal), lightDir));

	float3 diffuse = intensity;
	float3 specular = saturate(pow(intensity, 1));
	float3 ambient = 0.5;
	float4 ads = float4(diffuse + specular + ambient, 1);

	return ads;
}