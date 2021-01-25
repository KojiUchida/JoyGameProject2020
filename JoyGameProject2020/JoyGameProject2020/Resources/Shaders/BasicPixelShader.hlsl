#include "BasicShaderHeader.hlsli"

float4 PSmain(VSOutput input) : SV_TARGET {
	float4 texcolor = diffuseTex.Sample(smp, input.uv);

	float3 ambient = ambientColor;

	float3 intensity = dot(-lightDir, input.normal);
	float3 diffuse = saturate(intensity * diffuseColor);
	//float3 diffuseToon = saturate(step(0, intensity) * diffuseColor);

	float3 eyedir = normalize(cameraPos - input.worldpos.xyz);
	float3 reflect = normalize(lightDir + 2 * intensity * input.normal);
	float3 specular = pow(saturate(dot(reflect, eyedir)), shininess);
	float3 specularToon = smoothstep(0.4, 0.5, specular) * specularColor;
	float3 rim = saturate(smoothstep(0.6, 0.8, (1 - dot(eyedir, input.normal)) * pow(intensity, 0.2)));

	float4 ads;
	ads.rgb = ambient + (diffuse + specular) * lightColor;
	//ads.rgb = ambient + (diffuseToon + specularToon + rim) * lightColor;
	ads.a = 1;

	return ads * texcolor;
}