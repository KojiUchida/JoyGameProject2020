#include "ObjShaderHeader.hlsli"

float4 PSmain(VSOutput input) : SV_TARGET {
	float intensity = saturate(dot(normalize(input.normal), -lightDir));

	float4 diffuseTexColor = diffuseTex.Sample(smp, input.uv);
	float3 diffuse = intensity * diffuseColor;
	float3 specular = saturate(pow(intensity, shininess)) * specularColor;
	float3 ambient = ambientColor;
	float4 ads = float4(ambient + diffuse, alpha) * diffuseTexColor;
	ads.rgb += specular;

	return ads;
}