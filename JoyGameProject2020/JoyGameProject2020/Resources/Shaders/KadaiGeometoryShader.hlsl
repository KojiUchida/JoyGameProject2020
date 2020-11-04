#include "KadaiShaderHeader.hlsli"

static const uint vnum = 4;

static const float4 offset_array[vnum] = {
	float4(-0.5f, -0.5f, 0, 0),
	float4(-0.5f, 0.5f, 0, 0),
	float4(0.5f, -0.5f, 0, 0),
	float4(0.5f, 0.5f, 0, 0)
};

static const float2 uv_array[vnum] = {
	float2(0, 2),
	float2(0, 0),
	float2(2, 2),
	float2(2, 0)
};

[maxvertexcount(vnum)]
void GSmain(
	point VSOutput input[1] : SV_POSITION,
	inout TriangleStream< GSOutput > output
) {
	GSOutput element;
	for (uint i = 0; i < vnum; ++i) {
		element.svpos = input[0].pos + offset_array[i];
		element.svpos = mul(mvp, element.svpos);
		element.uv = uv_array[i];
		output.Append(element);
	}
}