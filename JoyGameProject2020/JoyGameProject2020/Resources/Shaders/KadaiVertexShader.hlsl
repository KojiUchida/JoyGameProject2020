#include "KadaiShaderHeader.hlsli"

VSOutput VSmain(float4 pos : POSITION) {
	VSOutput output;
	output.pos = pos;
	return output;
}